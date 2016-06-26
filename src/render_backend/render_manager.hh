#pragma once

# include "render_backend.hh"
# include "backend_plugins.hh"
# include "../base/manager.hh"

namespace render_backend
{
  class render_manager : public base::base_manager<render_backend>
  {
    public:
      render_manager();
      ~render_manager();

      bool init_render_backend();

      std::shared_ptr<resource::mesh>
        generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh);

      void render(std::shared_ptr<resource::mesh> mesh);
  };
}
