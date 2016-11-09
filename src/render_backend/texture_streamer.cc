#include "texture_streamer.hh"

namespace render_backend
{
  texture_streaming_job::texture_streaming_job(resource::streamed_texture* t, std::shared_ptr<resource::dds_loader> sl)
    : streamed_tex(t)
  {
    this->sl = sl;
  }

  std::deque<int> compute_mipmap_dimensions(int d)
  {
    std::deque<int> mips;

    while (d != 0)
    {
      mips.push_back(d);
      d /= 2;
    }

    return mips;
  }

  std::deque<int> compute_mipmap_offsets(int w, int h, int bs, int mips)
  {
    std::deque<int> w_mips = compute_mipmap_dimensions(w);
    std::deque<int> h_mips = compute_mipmap_dimensions(h);

    std::deque<int> mip_offsets;

    unsigned int size = 0;
    unsigned int offset = 0;
    for (int mip = 0; mip <= mips; mip++)
    {
      mip_offsets.push_back(offset);
      w = w_mips[mip];
      h = h_mips[mip];
      size = ((w + 3) / 4) * ((h + 3) / 4) * bs;

      offset += size;
    }

    return mip_offsets;
  }

  void texture_streaming_job::process(GLuint* pbo, int i, GLuint texture)
  {
    debug::log::get(debug::logINFO) << "Streaming texture \"" << streamed_tex->get_path() << "\"" << std::endl;

    if (!sl->load(streamed_tex->get_path().c_str()))
      return;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo[i]);
    glBindTexture(GL_TEXTURE_2D, texture);

    int w = sl->get_width();
    int h = sl->get_height();
    int bs = sl->get_block_size();
    unsigned int size;
    int mips = std::floor(std::log2(std::max(sl->get_width(), sl->get_width())));

    std::deque<int> w_mips = compute_mipmap_dimensions(w);
    std::deque<int> h_mips = compute_mipmap_dimensions(h);

    std::deque<int> mip_offsets = compute_mipmap_offsets(w, h, bs, mips);

    for (int mip = mips; mip >= 0; mip--)
    {
      w = w_mips[mip];
      h = h_mips[mip];
      size = ((w + 3) / 4) * ((h + 3) / 4) * bs;

      glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_DRAW);
      long* ptr = (long*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
      if (ptr)
      {
        std::memcpy(ptr, sl->get_generated_texture() + mip_offsets[mip], size);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        //if (sl->is_compressed())
        /*else
          glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sl->get_width(), sl->get_height(), GL_RGBA, GL_UNSIGNED_BYTE, 0);*/

        glCompressedTexSubImage2D(GL_TEXTURE_2D, mip, 0, 0, w, h, sl->get_format(), size, 0);
      }
      else
      {
        debug::log::get(debug::logERROR) << "Could not map the texture buffer" << std::endl;
        break;
      }
    }

    sl->clean();
    streamed_tex->set_loaded_tex(texture);
    debug::log::get(debug::logINDENT, 5) << "Bound to unit " << texture << std::endl;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  texture_streamer::texture_streamer()
    : end_streaming(false)
  {
    sl = std::make_shared<resource::dds_loader>();
  }

  void texture_streamer::init_shared_context(GLFWwindow* main_window)
  {
    this->main_window = main_window;

    generate_fake_texture();

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    generate_pbos(max_tex_size);

    anisotropy_level = opengl_pipeline_state::instance().get_state_of_lock("aniso_level");
  }

  void texture_streamer::query_texture_streaming_job(resource::streamed_texture* t)
  {
    debug::log::get(debug::logINFO) << "Querying a streaming job for " << t->get_path() << std::endl;

    if (!sl->load(t->get_path().c_str()))
      return;

    queue.push(texture_streaming_job(t, sl));

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy_level);
    int mips = std::floor(std::log2(std::max(sl->get_width(), sl->get_width())));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mips);

    int w = sl->get_width();
    int h = sl->get_height();
    int size = ((w + 3) / 4) * ((h + 3) / 4) * sl->get_block_size();
    std::deque<int> w_mips = compute_mipmap_dimensions(w);
    std::deque<int> h_mips = compute_mipmap_dimensions(h);
    int bs = sl->get_block_size();

    for (int mip = mips; mip >= 0; mip--)
    {
      w = w_mips[mip];
      h = h_mips[mip];
      size = ((w + 3) / 4) * ((h + 3) / 4) * bs;
      glCompressedTexImage2D(GL_TEXTURE_2D, mip, sl->get_format(), w, h, 0, size, nullptr);
    }

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
              job.process(pbos, i, tex);
              opengl_pipeline_state::instance().unlock();

              glFinish();

              i = 1 - i;
              std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            else
              std::this_thread::yield();
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
    if (!sl->load("res/tex/dds/fake_tex.dds"))
    {
      debug::log::get(debug::logERROR) << "Could not generate the fake texture" << std::endl;
      return;
    }
    glGenTextures(1, &fake_tex);
    glBindTexture(GL_TEXTURE_2D, fake_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    unsigned int size = ((sl->get_width()+3)/4)*((sl->get_height()+3)/4)*sl->get_block_size();
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, sl->get_format(), sl->get_width(), sl->get_height(), 0, size, sl->get_generated_texture());
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}
