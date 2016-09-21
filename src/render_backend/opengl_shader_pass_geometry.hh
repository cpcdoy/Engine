#pragma once

# include <memory>

# include "opengl_shader_pass.hh"

namespace render_backend
{
  class opengl_shader_pass_geometry : public opengl_shader_pass
  {
    public:
      opengl_shader_pass_geometry(std::string vs, std::string fs);
      virtual void process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long) override;

    private:
      GLuint g_buffer;
      GLuint g_position_depth, g_normal, g_albedo, g_metalness_roughness;
      GLuint metalness_map, roughness_map, albedo_map;
  };
}
