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
//# include "opengl_pipeline_state.hh"

namespace render_backend
{
  class opengl_shader_pass
  {
    public:
      opengl_shader_pass(std::string vs, std::string fs)
      {
        program = load_shaders(vs.c_str(), fs.c_str());

        w = opengl_pipeline_state::instance().get_state_of("width");
        h = opengl_pipeline_state::instance().get_state_of("height");
      }

      virtual ~opengl_shader_pass()
      {
        glUseProgram(0);
        glDeleteShader(program);
      }

      virtual void process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long rq_size = 1) = 0;

    protected:
      std::vector<GLint> uniforms;

      GLuint program;
      GLuint rbo_depth;

      int w, h;
  };
}
