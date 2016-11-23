#include "opengl_pipeline_state.hh"

namespace render_backend
{
  opengl_pipeline_state::opengl_pipeline_state()
  {
    states.insert(std::make_pair("shadow_map_res", 1024));
  }

  void opengl_pipeline_state::set_texture_streamer(std::shared_ptr<texture_streamer>& t)
  {
    tex_streamer = t;
  }

  void opengl_pipeline_state::query_texture_streaming_job(resource::streamed_texture* s)
  {
    tex_streamer->query_texture_streaming_job(s);
  }

  void opengl_pipeline_state::add_state(std::string s, long v)
  {
    states.insert(std::make_pair(s, v));
  }

  void opengl_pipeline_state::add_state_lock(std::string s, long v)
  {
    std::lock_guard<std::mutex> lock(mutex);

    add_state(s, v);
  }

  long opengl_pipeline_state::get_state_of(std::string s)
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

  long opengl_pipeline_state::get_state_of_lock(std::string s)
  {
    std::lock_guard<std::mutex> lock(mutex);

    return get_state_of(s);
  }

  void opengl_pipeline_state::lock()
  {
    mutex.lock();
  }

  void opengl_pipeline_state::unlock()
  {
    mutex.unlock();
  }

  opengl_shader_manager& opengl_pipeline_state::get_shader_manager()
  {
    return osm;
  }
}
