#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <memory>
# include <fstream>

# include "render_backend.hh"
# include "../resource/mesh.hh"
# include "../resource/gl_mesh.hh"
# include "../scene/camera.hh"

namespace render_backend
{
  class opengl_backend : public render_backend
  {
    public:
      opengl_backend();
      ~opengl_backend();

      virtual bool init_backend() override;

      virtual std::shared_ptr<resource::mesh> generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh) override;
      virtual void render(std::shared_ptr<resource::mesh> mesh) override;

      GLuint generate_vao(std::shared_ptr<resource::gl_mesh> mesh);

      GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path);
      virtual void update_renderer() override;
      virtual void set_ui_manager(std::shared_ptr<ui::ui_manager> ui) override;

      bool check_gl_extensions();

    private:
      std::vector<GLuint> programs;

      GLuint base_vao;

      std::vector<const unsigned char*> gl_caps;
  };
}
