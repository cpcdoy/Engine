#include "opengl_shader_pass_geometry.hh"

namespace render_backend
{
  opengl_shader_pass_geometry::opengl_shader_pass_geometry(std::string vs, std::string fs)
    : opengl_shader_pass(vs, fs)
  {
    glGenFramebuffers(1, &g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    // Position + linear depth buffer
    glGenTextures(1, &g_position_depth);
    glBindTexture(GL_TEXTURE_2D, g_position_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_BGRA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position_depth, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       debug::log::get(debug::log_level::logERROR) << "Position + linear depth buffer incomplete" << std::endl;
    // Normal color buffer
    glGenTextures(1, &g_normal);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       debug::log::get(debug::log_level::logERROR) << "Normal buffer incomplete" << std::endl;
    // Albedo buffer
    glGenTextures(1, &g_albedo);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       debug::log::get(debug::log_level::logERROR) << "Albedo buffer incomplete" << std::endl;
    // Metalness/Roughness buffer
    glGenTextures(1, &g_metalness_roughness_baked_ao);
    glBindTexture(GL_TEXTURE_2D, g_metalness_roughness_baked_ao);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_metalness_roughness_baked_ao, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       debug::log::get(debug::log_level::logERROR) << "Metalness/Roughness/Baked AO buffer incomplete" << std::endl;

    GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    uniforms.push_back(glGetUniformLocation(program, "model"));
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "view"));
    uniforms.push_back(glGetUniformLocation(program, "albedo_map"));
    uniforms.push_back(glGetUniformLocation(program, "metalness_map"));
    uniforms.push_back(glGetUniformLocation(program, "roughness_map"));
    uniforms.push_back(glGetUniformLocation(program, "baked_ao_map"));

    glUseProgram(program);

    glUniform1i(uniforms[3], 0);
    glUniform1i(uniforms[4], 1);
    glUniform1i(uniforms[5], 2);
    glUniform1i(uniforms[6], 3);

    opengl_pipeline_state::instance().add_state("g_buffer", g_buffer);
    opengl_pipeline_state::instance().add_state("g_normal", g_normal);
    opengl_pipeline_state::instance().add_state("g_albedo", g_albedo);
    opengl_pipeline_state::instance().add_state("g_metalness_roughness_baked_ao", g_metalness_roughness_baked_ao);
    opengl_pipeline_state::instance().add_state("g_position_depth", g_position_depth);
  }

  void opengl_shader_pass_geometry::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long rq_size)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &cam->get_projection_matrix()[0][0]);
    glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &cam->get_view_matrix()[0][0]);

    for (int i = 0; i < rq_size; i++)
    {
      auto m = render_queue[i];
      glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &m->get_model()[0][0]);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m->get_texture());

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, m->get_metalness_texture());

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, m->get_roughness_texture());

      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, m->get_ao_texture());

      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
    }
  }
}
