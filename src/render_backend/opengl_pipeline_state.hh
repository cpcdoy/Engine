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
      
      void add_state(std::string s, long v)
      {
        states.insert(std::make_pair(s, v));
      }

      long get_state_of(std::string s)
      {
        try
        {
          return states.at(s);
        }
        catch (std::out_of_range& e)
        {
          return 0;
        }
      }

    private:
      std::map<std::string, long> states;
  };
}
