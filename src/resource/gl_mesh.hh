#pragma once

# include <vector>
# include <memory>
# include <glm/glm.hpp>
# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif

# include "mesh.hh"

namespace resource
{
  struct gl_material
  {
      GLuint albedo_tex = 0;
      GLuint normal_tex = 0;
      GLuint metalness_tex = 0;
      GLuint roughness_tex = 0;
      GLuint ao_tex = 0;

      bool has_tex = false;

      glm::vec4 base_color;
  };

  class gl_mesh : public mesh
  {
    public:
      gl_mesh();
      gl_mesh(std::shared_ptr<mesh> m);
      ~gl_mesh();

      void set_vao(GLuint vao);
      GLuint get_vao();

      void set_texture(GLuint tex);
      GLuint get_texture();

      void set_normal_texture(GLuint tex);
      GLuint get_normal_texture();

      void set_roughness_texture(GLuint tex);
      GLuint get_roughness_texture();

      void set_metalness_texture(GLuint tex);
      GLuint get_metalness_texture();

      void set_ao_texture(GLuint tex);
      GLuint get_ao_texture();

      virtual void add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh) override;

    private:
      std::vector<GLuint> vaos;

      gl_material material;

      GLuint vertices_vbo;
      GLuint uv_vbo;
      GLuint normals_vbo;
  };
}
