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

# include "streamed_texture.hh"

namespace resource
{
  struct gl_material
  {
      GLuint albedo_tex = 0;
      GLuint normal_tex = 0;
      GLuint metalness_tex = 0;
      GLuint roughness_tex = 0;
      GLuint ao_tex = 0;

      std::shared_ptr<streamed_texture> albedo_streamed_tex;
      std::shared_ptr<streamed_texture> normal_streamed_tex;
      std::shared_ptr<streamed_texture> metalness_streamed_tex;
      std::shared_ptr<streamed_texture> roughness_streamed_tex;
      std::shared_ptr<streamed_texture> ao_streamed_tex;

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

      //Streamed texs
      void set_streamed_texture(std::shared_ptr<streamed_texture> tex);
      std::shared_ptr<streamed_texture>& get_streamed_texture();

      void set_streamed_normal_texture(std::shared_ptr<streamed_texture> tex);
      std::shared_ptr<streamed_texture>& get_streamed_normal_texture();

      void set_streamed_roughness_texture(std::shared_ptr<streamed_texture> tex);
      std::shared_ptr<streamed_texture>& get_streamed_roughness_texture();

      void set_streamed_metalness_texture(std::shared_ptr<streamed_texture> tex);
      std::shared_ptr<streamed_texture>& get_streamed_metalness_texture();

      void set_streamed_ao_texture(std::shared_ptr<streamed_texture> tex);
      std::shared_ptr<streamed_texture>& get_streamed_ao_texture();

      virtual void add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh) override;

    private:
      std::vector<GLuint> vaos;

      gl_material material;

      GLuint vertices_vbo;
      GLuint uv_vbo;
      GLuint normals_vbo;
  };
}
