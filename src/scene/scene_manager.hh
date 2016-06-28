#pragma once

# include <vector>
# include <memory>

# include "../debug/log.hh"
# include "../resource/resource_manager.hh"
# include "scene_graphs.hh"

namespace scene
{
  class scene_manager : public base::base_manager<scene_graph>
  {
    public:
      scene_manager(std::shared_ptr<resource::resource_manager> rm,
                    std::shared_ptr<ui::ui_manager> ui);
      ~scene_manager();
 
      bool init_scene_graph();

      std::shared_ptr<resource::mesh> create_node(std::shared_ptr<resource::mesh> mesh);

      std::shared_ptr<scene::camera> create_camera();
      void set_current_camera(std::shared_ptr<scene::camera> cam);
      std::shared_ptr<scene::camera> get_current_camera();

      std::vector<std::shared_ptr<resource::mesh>> get_render_queue();

    private:
      std::shared_ptr<resource::resource_manager> rm;
      std::shared_ptr<ui::ui_manager> ui;
  };
}
