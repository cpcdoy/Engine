#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <map>

# include "opengl_pipeline_state.hh"

namespace render_backend
{
  class opengl_pipeline_state
  {
    public:
      opengl_pipeline_state()
      {
        states.insert(std::make_pair("shadow_map_res", 1024));
      }

      static opengl_pipeline_state& instance()
      {
        static opengl_pipeline_state s;
        return s;
      }
      
      void add_state(std::string s, GLuint v)
      {
        states.insert(std::make_pair(s, v));
      }

      GLuint get_state_of(std::string s)
      {
        return states.at(s);
      }

    private:
      std::map<std::string, GLuint> states;
  };
}
