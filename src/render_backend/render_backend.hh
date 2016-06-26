#pragma once

# include <memory>
# include <string>

# include "../ui/ui_manager.hh"
# include "../resource/mesh.hh"
# include "../debug/log.hh"

namespace scene
{
  class scene_manager;
}

namespace render_backend
{
  class render_backend
  {
    public:
      render_backend()
      {
      }

      ~render_backend()
      {
      }

      virtual bool init_backend()
      {
        return false;
      }

      virtual std::string get_backend_id()
      {
        return backend_id;
      }

      virtual std::shared_ptr<resource::mesh> generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh)
      {
        return mesh;
      }

      virtual void render(std::shared_ptr<resource::mesh> mesh)
      {
        (void)mesh;
      }

      virtual void set_ui_manager(std::shared_ptr<ui::ui_manager> ui)
      {
        this->ui = ui;
      }

      virtual void update_renderer()
      {
      }

    protected:
      std::string backend_id = "virtual empty render backend";

      std::shared_ptr<ui::ui_manager> ui;
  };
}
