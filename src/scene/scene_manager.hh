#pragma once

# include <vector>
# include <memory>

# include "../debug/log.hh"
# include "../base/manager.hh"
# include "../resource/resource_manager.hh"
# include "scene_graphs.hh"

# define mesh_id unsigned int

namespace scene
{
  class scene_manager : public base::base_manager<scene_graph>
  {
    public:
      scene_manager(std::shared_ptr<resource::resource_manager> rm);
      ~scene_manager();
      
      bool init_scene_graph();
      mesh_id create_node(std::shared_ptr<resource::mesh> mesh);

      std::vector<std::shared_ptr<resource::mesh>> get_render_queue();

    private:
      std::shared_ptr<resource::resource_manager> rm;
  };
}
