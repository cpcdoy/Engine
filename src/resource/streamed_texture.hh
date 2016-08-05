#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <glm/glm.hpp>
# include <vector>
# include <memory>

# include "../render_backend/opengl_pipeline_state.hh"

namespace resource
{
  enum data_state
  {
    unloaded,
    loading,
    loaded
  };

  class streamed_texture
  {
    public:
      streamed_texture(GLuint fake_tex, std::string path, long unload_time=100)
        : state(data_state::unloaded),
          path(path),
          fake_tex(fake_tex),
          unload_time(unload_time)
      {
      }

      GLuint query_texture()
      {
        if (state == data_state::unloaded)
        {
          state = data_state::loading;
          render_backend::opengl_pipeline_state::instance().get_tex_streamer()->query_texture_streaming_job(this);
        }
        if (state == data_state::loaded)
          return loaded;
        else
          return fake_tex;
        
        last_used = glfwGetTime();
      }

      void unload_if_unused()
      {
        if (state == data_state::loaded && glfwGetTime() - last_used > unload_time)
        {
          glDeleteTextures(1, &loaded_tex);
          state = data_state::unloaded;
        }
      }

      std::string get_path()
      {
        return path;
      }

    protected:
      data_state state;

      std::string path;

      GLuint fake_tex;
      GLuint loaded_tex;

      long last_used;
      long unload_time = 100;
  };
}
