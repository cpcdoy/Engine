#include "opengl_shader_pass_geometry.hh"

namespace render_backend
{
  opengl_shader_pass_geometry::opengl_shader_pass_geometry(std::string vs, std::string fs)
    : opengl_shader_pass(vs, fs)
  {
    glGenFramebuffers(1, &g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    // Position + linear depth color buffer
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
    /*glGenTextures(1, &g_albedo);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       debug::log::get(debug::log_level::logERROR) << "Albedo buffer incomplete" << std::endl;*/

    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};//, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(2, attachments);

    uniforms.push_back(glGetUniformLocation(program, "model"));
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "view"));

    opengl_pipeline_state::instance().add_state("g_buffer", g_buffer);
    opengl_pipeline_state::instance().add_state("g_normal", g_normal);
    //opengl_pipeline_state::instance().add_state("g_albedo", g_albedo);
    opengl_pipeline_state::instance().add_state("g_position_depth", g_position_depth);
  }

  void opengl_shader_pass_geometry::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &cam->get_projection_matrix()[0][0]);
    glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &cam->get_view_matrix()[0][0]);

    for (auto m : render_queue)
    {
      glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &m->get_model()[0][0]);

      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
    }
  }
}
