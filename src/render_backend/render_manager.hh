#pragma once

# include "backend_plugins.hh"
# include "../base/manager.hh"

namespace scene
{
  class scene_manager;
}

namespace resource
{
  class resource_manager;
  class mesh;
}

namespace render_backend
{
  class render_backend;
  class render_manager : public base::base_manager<render_backend>
  {
    public:
      render_manager();
      ~render_manager();

      bool init_render_backend(int w, int h);

      std::shared_ptr<resource::mesh>
        generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh);

      void render();
      void batch(std::shared_ptr<scene::scene_manager> sm);

      void set_ui_manager(std::shared_ptr<ui::ui_manager> ui);
      void set_camera(std::shared_ptr<scene::camera> cam);
      void update_renderer();
      void add_state(std::string s, long r);
  };
}
