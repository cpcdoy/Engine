#pragma once

# include <memory>

# include "opengl_shader_pass.hh"

namespace render_backend
{
  class opengl_shader_pass_atmosphere : public opengl_shader_pass
  {
    public:
      opengl_shader_pass_atmosphere(std::string vs, std::string fs);
      virtual void process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long rq_size) override;

    private:
      std::shared_ptr<resource::gl_mesh> sky_dome;
      std::vector<glm::vec3> create_skydome_with_atmosphere(float r_inner, float r_outer);
  };
}
