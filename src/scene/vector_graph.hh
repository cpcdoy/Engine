#pragma once

# include <memory>

# include "../debug/log.hh"
# include "../resource/mesh.hh"
# include "scene_graph.hh"

# include "scene_manager.hh"

namespace scene
{
  class vector_graph : public scene_graph
  {
    public:
      vector_graph();

      virtual ~vector_graph() override;

      virtual bool init_scene_graph() override;

      virtual std::shared_ptr<resource::mesh> create_node(std::shared_ptr<resource::mesh> mesh) override;
      virtual std::shared_ptr<scene::camera> create_camera() override;

    private:
      std::vector<std::shared_ptr<resource::mesh>> meshes;

      std::vector<std::shared_ptr<scene::camera>> cams;
  };
}
