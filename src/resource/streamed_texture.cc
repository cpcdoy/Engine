#include "streamed_texture.hh"

namespace resource
{
  streamed_texture::streamed_texture(GLuint fake_tex, std::string path, long unload_time)
    : state(data_state::unloaded),
    path(path),
    fake_tex(fake_tex),
    unload_time(unload_time)
  {
  }

  GLuint streamed_texture::query_texture()
  {
    if (state == data_state::unloaded)
    {
      state = data_state::loading;
      render_backend::opengl_pipeline_state::instance().query_texture_streaming_job(this);
    }
    if (state == data_state::loaded)
      return loaded_tex;
    else
      return fake_tex;

    last_used = glfwGetTime();
  }

  void streamed_texture::set_loaded_tex(GLuint tex)
  {
    loaded_tex = tex;
  }
  void streamed_texture::set_state(data_state st)
  {
    state = st;
  }

  void streamed_texture::unload_if_unused()
  {
    if (state == data_state::loaded && glfwGetTime() - last_used > unload_time)
    {
      glDeleteTextures(1, &loaded_tex);
      state = data_state::unloaded;
    }
  }

  std::string streamed_texture::get_path()
  {
    return path;
  }
}
