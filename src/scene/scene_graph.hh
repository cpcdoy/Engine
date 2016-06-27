#pragma once

# include <memory>

# include "../debug/log.hh"
# include "../resource/mesh.hh"

# define mesh_id unsigned int

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

      virtual mesh_id create_node(std::shared_ptr<resource::mesh> mesh)
      {
        (void)mesh;

        return 0;
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

      std::vector<std::shared_ptr<resource::mesh>> meshes;

      std::vector<std::shared_ptr<resource::mesh>> render_queue;
  };
}
