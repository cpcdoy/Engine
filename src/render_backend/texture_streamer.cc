#include "texture_streamer.hh"

namespace render_backend
{
  texture_streaming_job::texture_streaming_job(resource::streamed_texture* t, std::shared_ptr<resource::soil_loader> sl)
    : streamed_tex(t)
  {
    this->sl = sl;
  }

  void texture_streaming_job::process(GLuint pbo, GLuint texture)
  {
    debug::log::get(debug::logINFO) << "Streaming texture \"" << streamed_tex->get_path() << "\"" << std::endl;

    if (!sl->load(streamed_tex->get_path().c_str()))
      return;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBindTexture(GL_TEXTURE_2D, texture);

    int size = sl->get_width() * sl->get_height() * 4;
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_DRAW);
    long* ptr = (long*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr)
    {
      std::memcpy(ptr, sl->get_generated_texture(), size);
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sl->get_width(), sl->get_height(), GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    else
      debug::log::get(debug::logERROR) << "Could not map the texture buffer" << std::endl;

    streamed_tex->set_loaded_tex(texture);
    streamed_tex->set_state(resource::data_state::loaded);

    debug::log::get(debug::logINDENT, 5) << "Bound to unit " << texture << std::endl;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  texture_streamer::texture_streamer()
  {
    sl = std::make_shared<resource::soil_loader>();
  }

  void texture_streamer::init_shared_context(GLFWwindow* main_window)
  {
    this->main_window = main_window;

    generate_fake_texture();

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    generate_pbos(max_tex_size);
  }

  void texture_streamer::query_texture_streaming_job(resource::streamed_texture* t)
  {
    debug::log::get(debug::logINFO) << "Querying a streaming job for " << t->get_path() << std::endl;
    queue.push(texture_streaming_job(t, sl));

    if (!sl->load(t->get_path().c_str()))
      return;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, sl->get_width(), sl->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    sl->clean();
    texture_binding_pool.push(texture);
  }

  std::shared_ptr<resource::streamed_texture> texture_streamer::query_streamed_texture(std::string path)
  {
    return std::make_shared<resource::streamed_texture>(fake_tex, path);
  }

  void texture_streamer::set_fake_window(GLFWwindow* w)
  {
    tex_streamer_fake_window = w;
  }

  void texture_streamer::stream()
  {
    data_streamer_thread = std::thread([&]()
        {
          glfwMakeContextCurrent(tex_streamer_fake_window);
          glfwWindowHint(GLFW_VISIBLE, false);

          int i = 0;

          while (!end_streaming)
            if (!empty())
            {
              texture_streaming_job job = pop();

              GLuint tex = texture_binding_pool.pop();

              opengl_pipeline_state::instance().lock();
              job.process(pbos[i], tex);
              opengl_pipeline_state::instance().unlock();

              glFlush();

              i = 1 - i;
              //std::this_thread::sleep_for(std::chrono::milliseconds(400));
            }
            else
              std::this_thread::sleep_for(std::chrono::seconds(1));
        });
  }

  void texture_streamer::generate_pbos(int size)
  {
    glGenBuffers(2, pbos);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbos[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbos[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_DRAW);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void texture_streamer::generate_fake_texture()
  {
    debug::log::get(debug::logINFO) << "Generating fake texture" << std::endl;
    if (!sl->load("res/tex/fake_tex.jpg"))
    {
      debug::log::get(debug::logERROR) << "Could not generate the fake texture" << std::endl;
      return;
    }
    glGenTextures(1, &fake_tex);
    glBindTexture(GL_TEXTURE_2D, fake_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, sl->get_width(), sl->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sl->get_generated_texture());
    //glGenerateMipmap(GL_TEXTURE_2D);
  }
}
