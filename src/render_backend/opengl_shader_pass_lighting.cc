#include "opengl_shader_pass_lighting.hh"

namespace render_backend
{
  opengl_shader_pass_lighting::opengl_shader_pass_lighting(std::string vs, std::string fs)
  : opengl_shader_pass(vs, fs)
  {
    uniforms.push_back(glGetUniformLocation(program, "view"));
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "light_pos"));
    uniforms.push_back(glGetUniformLocation(program, "view_pos"));
    uniforms.push_back(glGetUniformLocation(program, "light_space_matrix"));
    uniforms.push_back(glGetUniformLocation(program, "shadow_map"));
    uniforms.push_back(glGetUniformLocation(program, "ao_map"));
    uniforms.push_back(glGetUniformLocation(program, "model"));

    uniforms.push_back(glGetUniformLocation(program, "screen_res"));

    glUseProgram(program);

    glUniform1i(uniforms[5], 0);
    glUniform1i(uniforms[6], 1);

    glUniform2fv(uniforms.back(), 1, &glm::vec2(w, h)[0]);
  }

  void opengl_shader_pass_lighting::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    auto projection = cam->get_projection_matrix();
    auto view = cam->get_view_matrix();

    //
    glm::vec3 lightPos = glm::vec3(-5, 10, -5);
    lightPos.z = cos(glfwGetTime()) * 2.0f;

    float area = 10;
    glm::mat4 depth_proj = glm::ortho(-area, area, -area, area, -10.0f, 100.0f);
    glm::mat4 depth_view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));

    glm::mat4 depth_VP = depth_proj * depth_view;
    //

    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &projection[0][0]);
    // Set light uniforms
    glUniform3fv(uniforms[2], 1, &lightPos[0]);
    glUniform3fv(uniforms[3], 1, &cam->get_camera_position()[0]);
    glUniformMatrix4fv(uniforms[4], 1, GL_FALSE, &depth_VP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("depth_texture"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("ssao_texture"));

    for (auto m : render_queue)
    {
      glUniformMatrix4fv(uniforms[7], 1, GL_FALSE, &m->get_model()[0][0]);

      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
    }
  }
}
