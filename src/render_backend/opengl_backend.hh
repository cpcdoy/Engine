#pragma once

# include <GL/glew.h>

# include "render_backend.hh"

namespace render_backend
{
  class opengl_backend : public render_backend
  {
    public:
      opengl_backend();
      ~opengl_backend();

      virtual bool init_backend() override;
  };
}
