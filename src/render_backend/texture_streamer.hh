#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <iostream>

# include "streamer.hh"
# include "../resource/streamed_texture.hh"
# include "../resource/soil_loader.hh"

namespace render_backend
{
  class texture_streaming_job
  {
    public:
      texture_streaming_job(resource::streamed_texture* t)
        : streamed_tex(t)
      {
        sl = std::make_shared<resource::soil_loader>();
      }

      void process(GLuint pbo)
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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
        }
        else
          debug::log::get(debug::logERROR) << "Error mapping texture buffer" << std::endl;

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        //glBindTexture(GL_TEXTURE_2D, 0);
      }

    protected:
      resource::streamed_texture* streamed_tex;

      std::shared_ptr<resource::soil_loader> sl;
  };

  class texture_streamer : public streamer<texture_streaming_job>
  {
    public:
      texture_streamer()
      {
        sl = std::make_shared<resource::soil_loader>();
      }

      void init_shared_context(GLFWwindow* main_window)
      {
        this->main_window = main_window;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        generate_pbos(max_tex_size);

        generate_fake_texture();
      }

      void query_texture_streaming_job(resource::streamed_texture* t)
      {
        queue.push(texture_streaming_job(t));
      }

      std::shared_ptr<resource::streamed_texture> query_streamed_texture(std::string path)
      {
        return std::make_shared<resource::streamed_texture>(fake_tex, path);
      }

      virtual void stream() override
      {
        data_streamer_thread = std::thread([=]()
            {
              tex_streamer_fake_window = glfwCreateWindow(1, 1, "fake", nullptr, main_window);

              glfwMakeContextCurrent(tex_streamer_fake_window);
              glfwWindowHint(GLFW_VISIBLE, false);

              int i = 0;

              while (true)
              {
                texture_streaming_job job = pop();
                job.process(pbos[i]);

                i = 1 - i;
              }
            });
      }

    protected:
      void generate_pbos(int size)
      {
        glGenBuffers(2, pbos);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbos[0]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_COPY);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbos[1]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_COPY);
      }

      void generate_fake_texture()
      {
        if (!sl->load("res/tex/fake_tex.jpg"))
        {
          std::cout << "errorrrr" << std::endl;
          return;
        }

        glGenTextures(1, &fake_tex);
        glBindTexture(GL_TEXTURE_2D, fake_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, sl->get_width(), sl->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sl->get_generated_texture());
        //glGenerateMipmap(GL_TEXTURE_2D);

        std::cout << (long)sl->get_generated_texture() << " " << (int)sl->get_generated_texture()[0] << " " << fake_tex << std::endl;

      }

      GLFWwindow* tex_streamer_fake_window = nullptr;
      GLFWwindow* main_window = nullptr;

      GLuint pbos[2];

      GLuint fake_tex;
      
      int max_tex_size = 2048*2048*4*sizeof(GLubyte);

      std::thread data_streamer_thread;

      std::shared_ptr<resource::soil_loader> sl;
  };
}
