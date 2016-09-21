#include "opengl_shader_pass_lighting.hh"

namespace render_backend
{
  opengl_shader_pass_lighting::opengl_shader_pass_lighting(std::string vs, std::string fs)
  : opengl_shader_pass(vs, fs)
  {
    uniforms.push_back(glGetUniformLocation(program, "view")); // 0
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "light_pos"));
    uniforms.push_back(glGetUniformLocation(program, "view_pos"));
    uniforms.push_back(glGetUniformLocation(program, "light_space_matrix"));
    uniforms.push_back(glGetUniformLocation(program, "shadow_map")); // 5
    uniforms.push_back(glGetUniformLocation(program, "ao_map"));
    uniforms.push_back(glGetUniformLocation(program, "diffuse_map"));
    uniforms.push_back(glGetUniformLocation(program, "metalness_roughness_map"));
    //uniforms.push_back(glGetUniformLocation(program, "roughness_map"));
    uniforms.push_back(glGetUniformLocation(program, "baked_ao_map")); // 9
    uniforms.push_back(glGetUniformLocation(program, "model"));
    uniforms.push_back(glGetUniformLocation(program, "trans_inv_model"));
    uniforms.push_back(glGetUniformLocation(program, "screen_res")); // 12

    glUseProgram(program);

    glUniform1i(uniforms[5], 0);
    glUniform1i(uniforms[6], 1);
    glUniform1i(uniforms[7], 2);
    //glUniform1i(uniforms[8], 3);
    glUniform1i(uniforms[8], 3);
    glUniform1i(uniforms[9], 4);

    glUniform2fv(uniforms.back(), 1, &glm::vec2(w, h)[0]);
  }

  void opengl_shader_pass_lighting::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long rq_size)
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

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_albedo"));

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_metalness_roughness"));

    for (int i = 0; i < rq_size; i++)
    {
      const auto& m = render_queue[i];
      glUniformMatrix4fv(uniforms[10], 1, GL_FALSE, &m->get_model()[0][0]);
      glUniformMatrix3fv(uniforms[11], 1, GL_FALSE, &glm::transpose(glm::inverse(glm::mat3(m->get_model())))[0][0]);

      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, m->get_ao_texture());
      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
    }
  }
}
