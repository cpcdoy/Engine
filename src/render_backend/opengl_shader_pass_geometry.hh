#pragma once

# include <memory>

# include "opengl_shader_pass.hh"

namespace render_backend
{
  class opengl_shader_pass_geometry : public opengl_shader_pass
  {
    public:
      opengl_shader_pass_geometry(std::string vs, std::string fs);
      virtual void process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam) override;
  };
}
