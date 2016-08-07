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
      streamed_texture(GLuint fake_tex, std::string path, long unload_time=100);

      GLuint query_texture();
      
      void unload_if_unused();

      std::string get_path();
      void set_loaded_tex(GLuint tex);
      void set_state(data_state st);

    protected:
      data_state state;

      std::string path;

      GLuint fake_tex;
      GLuint loaded_tex;

      long last_used;
      long unload_time = 100;
  };
}
