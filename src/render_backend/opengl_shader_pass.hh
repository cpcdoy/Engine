#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif

# include <vector>
# include <string>

# include "opengl_backend_utils.hh"
# include "../resource/gl_mesh.hh"
# include "../scene/camera.hh"
//# include "../resource/streamed_texture.hh"
# include "opengl_pipeline_state.hh"
# include "../event/events.hh"

namespace render_backend
{
  class opengl_shader_pass
  {
    public:
      opengl_shader_pass(std::string vs, std::string fs, std::string tcs = "", std::string tes = "", std::string gs = "", std::string cs = "")
      {
        program = opengl_pipeline_state::instance().get_shader_manager().compile_shaders(vs, fs, tcs, tes, gs, cs);

        w = opengl_pipeline_state::instance().get_state_of("width");
        h = opengl_pipeline_state::instance().get_state_of("height");

        event::channel::add<event::performance_statistics_event>(this);
      }

      virtual ~opengl_shader_pass()
      {
      }

      virtual void process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long rq_size = 1) = 0;

      virtual void operator()(const event::performance_statistics_event& event) = 0;

    protected:
      std::vector<GLint> uniforms;

      GLuint program;
      GLuint rbo_depth;

      int w, h;
  };
}
