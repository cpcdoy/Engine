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
# include "texture_kind.hh"

namespace resource
{
  struct gl_material
  {
      std::array<GLuint, 6> tex_ids;
      std::array<std::shared_ptr<streamed_texture>, 6> streamed_texs;

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

      //GL tex ids
      void set_texture(GLuint tex, texture_kind kind = texture_kind::ALBEDO);
      GLuint get_texture(texture_kind kind = texture_kind::ALBEDO);

      //Streamed texs
      void set_streamed_texture(std::shared_ptr<streamed_texture> tex, texture_kind kind = texture_kind::ALBEDO);
      std::shared_ptr<streamed_texture>& get_streamed_texture(texture_kind kind = texture_kind::ALBEDO);

      virtual void add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh) override;
      virtual void query_texture_unloading() override;

    private:
      std::vector<GLuint> vaos;

      gl_material material;
  };
}
