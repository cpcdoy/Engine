#include "opengl_shader_pass_shadow_map.hh"

namespace render_backend
{
  opengl_shader_pass_shadow_map::opengl_shader_pass_shadow_map(std::string vs, std::string fs)
  : opengl_shader_pass(vs, fs)
  {
    glGenFramebuffers(1, &detph_fbo);

    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);

    shadow_map_res = opengl_pipeline_state::instance().get_state_of("shadow_map_res");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadow_map_res, shadow_map_res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, detph_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,  depth_texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      debug::log::get(debug::log_level::logERROR) << "Shadow map buffer incomplete" << std::endl;

    uniforms.push_back(glGetUniformLocation(program, "model"));
    uniforms.push_back(glGetUniformLocation(program, "VP"));

    opengl_pipeline_state::instance().add_state("depth_texture", depth_texture);
  }

  void opengl_shader_pass_shadow_map::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera>, long)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, detph_fbo);
    glViewport(0, 0, shadow_map_res, shadow_map_res);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glm::vec3 lightPos = glm::vec3(-5, 11, -5);
    lightPos.z = cos(glfwGetTime()) * 2.0f;

    float area = 10;
    glm::mat4 depth_proj = glm::ortho(-area, area, -area, area, -10.0f, 100.0f);
    glm::mat4 depth_view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));

    glm::mat4 depth_model = glm::mat4(1.0);
    glm::mat4 depth_VP = depth_proj * depth_view;

    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &depth_model[0][0]);
    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &depth_VP[0][0]);

    for (auto m : render_queue)
    {
      glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &m->get_model()[0][0]);

      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
    }

    glViewport(0, 0, w, h);
  }
}
