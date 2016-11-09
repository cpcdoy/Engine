#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <memory>
# include <map>
# include <mutex>

# include "texture_streamer.hh"

namespace render_backend
{
  class texture_streamer;

  class opengl_pipeline_state
  {
    public:
      opengl_pipeline_state();

      static opengl_pipeline_state& instance()
      {
        static opengl_pipeline_state s;
        return s;
      }

      void set_texture_streamer(std::shared_ptr<texture_streamer>& t);

      void query_texture_streaming_job(resource::streamed_texture* s);

      void add_state(std::string s, long v);
      void add_state_lock(std::string s, long v);

      long get_state_of(std::string s);
      long get_state_of_lock(std::string s);

      void lock();
      void unlock();

    private:
      std::map<std::string, long> states;

      std::shared_ptr<texture_streamer> tex_streamer;

      std::mutex mutex;
  };
}
