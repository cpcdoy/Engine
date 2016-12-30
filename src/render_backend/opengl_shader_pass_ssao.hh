#pragma once

# include <memory>
# include <random>

# include "opengl_shader_pass.hh"

namespace render_backend
{
  class opengl_shader_pass_ssao : public opengl_shader_pass
  {
    public:
      opengl_shader_pass_ssao(std::string vs, std::string fs);
      virtual void process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long) override;
      virtual void operator()(const event::performance_statistics_event& event) override;

    private:
      std::vector<glm::vec3> ssao_kernel;
      std::vector<glm::vec3> ssao_noise;

      GLuint noise_texture;
      GLuint quad_vao;
      GLuint ssao_fbo;

      unsigned int kernel_size = 16;
  };
}
