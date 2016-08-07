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
}
