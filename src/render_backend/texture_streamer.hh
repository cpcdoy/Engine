#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <iostream>

# include "streamer.hh"
# include "../resource/soil_loader.hh"

namespace resource
{
  class streamed_texture;
}

#include "../resource/streamed_texture.hh"

namespace render_backend
{
  class texture_streaming_job
  {
    public:
      texture_streaming_job(resource::streamed_texture* t, std::shared_ptr<resource::soil_loader> sl);

      void process(GLuint pbo, GLuint texture);

    protected:
      resource::streamed_texture* streamed_tex;

      std::shared_ptr<resource::soil_loader> sl;
  };

  class texture_streamer : public streamer<texture_streaming_job>
  {
    public:
      texture_streamer();

      void init_shared_context(GLFWwindow* main_window);

      void query_texture_streaming_job(resource::streamed_texture* t);

      std::shared_ptr<resource::streamed_texture> query_streamed_texture(std::string path);

      virtual void stream() override;
      void set_fake_window(GLFWwindow* w);

    protected:
      void generate_pbos(int size);

      void generate_fake_texture();

      GLFWwindow* tex_streamer_fake_window = nullptr;
      GLFWwindow* main_window = nullptr;

      GLuint pbos[2];

      GLuint fake_tex;
      
      int max_tex_size = 2048*2048*4*sizeof(GLubyte);

      std::thread data_streamer_thread;

      std::shared_ptr<resource::soil_loader> sl;

      bool end_streaming = false;

      streaming_queue<GLuint> texture_binding_pool;
  };
}
