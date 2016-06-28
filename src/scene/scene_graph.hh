#pragma once

# include <memory>

# include "../ui/ui_manager.hh"
# include "../debug/log.hh"
# include "../resource/mesh.hh"
# include "../scene/camera.hh"

namespace scene
{
  class scene_graph
  {
    public:
      scene_graph()
      {
      }

      virtual ~scene_graph()
      {
      }

      virtual bool init_scene_graph()
      {
        return false;
      }

      virtual std::shared_ptr<resource::mesh> create_node(std::shared_ptr<resource::mesh> mesh)
      {
        return mesh;
      }

      virtual std::shared_ptr<scene::camera> create_camera()
      {
        auto cam = std::make_shared<scene::camera>();
        cam->set_window_context(ui);

        return cam;
      }

      virtual void set_current_camera(std::shared_ptr<scene::camera> cam)
      {
        current_cam = cam;
      }

      virtual void set_ui_manager(std::shared_ptr<ui::ui_manager> ui)
      {
        this->ui = ui;
      }

      virtual std::shared_ptr<scene::camera> get_current_camera()
      {
        return current_cam;
      }

      virtual std::string get_scene_graph_id()
      {
        return scene_graph_id;
      }

      virtual std::vector<std::shared_ptr<resource::mesh>> get_render_queue()
      {
        return render_queue;
      }

    protected:
      std::string scene_graph_id = "virtual empty scene graph";

      std::shared_ptr<scene::camera> current_cam;
      std::vector<std::shared_ptr<resource::mesh>> render_queue;

      std::shared_ptr<ui::ui_manager> ui;
  };
}
