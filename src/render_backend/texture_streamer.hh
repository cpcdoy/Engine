#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <iostream>
# include <cstring>
# include <condition_variable>
# include <mutex>

# include "../resource/streamed_texture.hh"
# include "streamer.hh"
# include "../resource/dds_loader.hh"
# include "../resource/soil_loader.hh"
# include "../event/events.hh"

namespace resource
{
  class streamed_texture;
}

namespace render_backend
{
  class texture_streaming_job
  {
    public:
      texture_streaming_job(resource::streamed_texture* t, std::shared_ptr<resource::dds_loader> sl);

      void process(GLuint* pbo, int i, GLuint texture);

    protected:
      resource::streamed_texture* streamed_tex;

      std::shared_ptr<resource::dds_loader> sl;
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

      void operator()(const event::texture_streaming_event& event);

    protected:
      void generate_pbos(int size);

      void generate_fake_texture();

      GLFWwindow* tex_streamer_fake_window = nullptr;
      GLFWwindow* main_window = nullptr;

      GLuint pbos[2];
      int i = 0;

      GLuint fake_tex;

      int max_tex_size = 2048 * 2048 * 4 * sizeof(GLubyte);

      std::thread data_streamer_thread;

      std::shared_ptr<resource::dds_loader> sl;

      std::atomic<bool> end_streaming;
      std::condition_variable cond_var;
      std::mutex queue_mutex;

      concurrent_queue<GLuint> texture_binding_pool;

      float anisotropy_level = 0.0f;
      event::texture_streaming_event event;
  };
}
