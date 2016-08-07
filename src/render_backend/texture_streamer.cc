#include "texture_streamer.hh"

namespace render_backend
{
  texture_streaming_job::texture_streaming_job(resource::streamed_texture* t, std::shared_ptr<resource::soil_loader> sl)
    : streamed_tex(t)
  {
    this->sl = sl;
  }

  void texture_streaming_job::process(GLuint pbo)
  {
    debug::log::get(debug::logINDENT, 5) << "Streaming texture \"" << streamed_tex->get_path() << "\"" << std::endl;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);

    if (!sl->load(streamed_tex->get_path().c_str()))
      return;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, sl->get_width(), sl->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sl->get_generated_texture());
    //glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sl->get_width(), sl->get_height(), GL_RGBA, GL_UNSIGNED_BYTE, 0);

    int size = sl->get_width() * sl->get_height() * 4;
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_COPY);
    long* ptr = (long*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr)
    {
      for (long j = 0; j < size / 8; j++)
        ptr[j] = sl->get_generated_texture()[j];
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

      streamed_tex->set_loaded_tex(texture);
      streamed_tex->set_state(resource::data_state::loaded);
    }
    else
      debug::log::get(debug::logERROR) << "Could not map the texture buffer" << std::endl;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    //glBindTexture(GL_TEXTURE_2D, 0);
  }

  texture_streamer::texture_streamer()
  {
    sl = std::make_shared<resource::soil_loader>();
  }

  void texture_streamer::init_shared_context(GLFWwindow* main_window)
  {
    this->main_window = main_window;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    generate_pbos(max_tex_size);

    generate_fake_texture();
  }

  void texture_streamer::query_texture_streaming_job(resource::streamed_texture* t)
  {

    debug::log::get(debug::logINFO) << "Querying a streaming job for " << t->get_path() << std::endl;
    queue.push(texture_streaming_job(t, sl));
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
    data_streamer_thread = std::thread([=]()
        {
          glfwMakeContextCurrent(tex_streamer_fake_window);
          glfwWindowHint(GLFW_VISIBLE, false);

          int i = 0;

          while (!end_streaming)
            if (!empty())
            {
              texture_streaming_job job = pop();
              job.process(pbos[i]);

              glFlush();

              i = 1 - i;
            }
        });
  }

  void texture_streamer::generate_pbos(int size)
  {
    glGenBuffers(2, pbos);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbos[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_COPY);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbos[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_COPY);
  }

  void texture_streamer::generate_fake_texture()
  {
    if (!sl->load("res/tex/fake_tex.jpg"))
    {
      debug::log::get(debug::logERROR) << "Could not generate the fake texture" << std::endl;
      return;
    }
    glEnable(GL_TEXTURE_2D);
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
