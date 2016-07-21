#pragma once

# include <memory>
# include <string>

# include "../resource/resource_manager.hh"
# include "../scene/scene_manager.hh"

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

      virtual bool init_backend(int, int)
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

      virtual void set_compatible_texture(std::shared_ptr<resource::mesh>&, unsigned char*, int, int, texture_kind)
      {
      }

      virtual void batch(std::shared_ptr<scene::scene_manager>)
      {
      }

      virtual void render()
      {
      }

      virtual void set_ui_manager(std::shared_ptr<ui::ui_manager> ui)
      {
        this->ui = ui;
      }

      virtual void set_camera(std::shared_ptr<scene::camera> cam)
      {
        this->cam = cam;
      }

      virtual void update_renderer()
      {
      }

      virtual void add_state(std::string, long)
      {
      }

      virtual void set_clear_color(float, float, float)
      {
      }

    protected:
      std::string backend_id = "virtual empty render backend";

      std::shared_ptr<ui::ui_manager> ui;
      std::shared_ptr<scene::camera> cam;
  };
}
