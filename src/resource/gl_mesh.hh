#pragma once

# include <vector>
# include <memory>
# include <glm/glm.hpp>
# include <GLFW/glfw3.h>

# include "mesh.hh"

namespace resource
{
  class gl_mesh : public mesh
  {
    public:
      gl_mesh();
      gl_mesh(std::shared_ptr<mesh> m);
      ~gl_mesh();

      void set_vao(GLuint vao);
      GLuint get_vao();

    private:
      GLuint vao;

      GLuint vertices_vbo;
      GLuint uv_vbo;
      GLuint normals_vbo;
  };
}
