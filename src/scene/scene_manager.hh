#pragma once

# include <vector>
# include <memory>
# include <algorithm>

# include "../debug/log.hh"
# include "scene_graphs.hh"

namespace resource
{
  class resource_manager;
}

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

      std::vector<std::shared_ptr<resource::mesh>>& get_render_queue();
      size_t compute_view_frustum_culling(std::vector<std::shared_ptr<resource::mesh>>& rq, std::shared_ptr<camera>& cam);
      int get_render_queue_size();
      void set_render_queue_size(int);

      void operator()(const event::engine_stop_event&)
      {
        debug::log::get(debug::logINFO) << "Scene manager shutting down..." << std::endl;
      }

    private:
      std::shared_ptr<resource::resource_manager> rm;
      std::shared_ptr<ui::ui_manager> ui;
  };
}
