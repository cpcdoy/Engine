#include "opengl_shader_pass_geometry.hh"

namespace render_backend
{
  opengl_shader_pass_geometry::opengl_shader_pass_geometry(std::string vs, std::string fs)
    : opengl_shader_pass(vs, fs)
  {
    uniforms.push_back(glGetUniformLocation(program, "model"));
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "view"));
  }

  void opengl_shader_pass_geometry::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam)
  {
    for (auto m : render_queue)
    {
      glUseProgram(program);

      glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &m->get_model()[0][0]);
      glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &cam->get_projection_matrix()[0][0]);
      glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &cam->get_view_matrix()[0][0]);

      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
    }
    glBindVertexArray(0);
  }
}
