#pragma once

# include <GL/glew.h>
# include <memory>

# include "render_backend.hh"
# include "../resource/mesh.hh"
# include "../resource/gl_mesh.hh"

namespace render_backend
{
  class opengl_backend : public render_backend
  {
    public:
      opengl_backend();
      ~opengl_backend();

      virtual bool init_backend() override;

      virtual std::shared_ptr<resource::mesh> generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh) override;
      virtual void render(std::shared_ptr<resource::mesh> mesh) override;

      GLuint generate_vao();
  };
}
