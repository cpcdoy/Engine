#pragma once

# include <memory>

# include "../debug/log.hh"
# include "../resource/mesh.hh"
# include "scene_graph.hh"

# include "scene_manager.hh"

# define mesh_id unsigned int

namespace scene
{
  class vector_graph : public scene_graph
  {
    public:
      vector_graph();

      virtual ~vector_graph() override;

      virtual bool init_scene_graph() override;

      virtual mesh_id create_node(std::shared_ptr<resource::mesh> mesh) override;
  };
}
