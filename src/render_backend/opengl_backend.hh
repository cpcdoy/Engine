#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <memory>
# include <fstream>
# include <algorithm>

# include "../scene/camera.hh"
# include "render_backend.hh"
# include "../resource/mesh.hh"
# include "../resource/gl_mesh.hh"
# include "opengl_backend_utils.hh"
# include "opengl_shader_pass.hh"
# include "opengl_shader_pass_no_lighting.hh"

namespace render_backend
{
  class render_backend;
}

namespace render_backend
{
  class opengl_backend : public render_backend
  {
    public:
      opengl_backend();
      ~opengl_backend();

      virtual bool init_backend() override;

      virtual std::shared_ptr<resource::mesh> generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh) override;
      virtual void batch(std::shared_ptr<scene::scene_manager> sm) override;
      virtual void render() override;

      GLuint generate_vao(std::shared_ptr<resource::gl_mesh> mesh);

      virtual void update_renderer() override;
      virtual void set_ui_manager(std::shared_ptr<ui::ui_manager> ui) override;

      bool check_gl_extensions();
      std::vector<std::string> get_gl_extensions();

    private:
      std::vector<GLuint> programs;

      GLuint base_vao;
      std::vector<GLuint> vaos;

      std::vector<std::string> gl_caps;

      std::vector<std::shared_ptr<resource::gl_mesh>> render_queue;
      std::vector<std::shared_ptr<opengl_shader_pass>> pipeline;
  };
}
