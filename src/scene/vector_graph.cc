#include "vector_graph.hh"

namespace scene
{
  vector_graph::vector_graph()
    : scene_graph()
  {
    scene_graph_id = "vector scene graph";
  }

  vector_graph::~vector_graph()
  {
    meshes.clear();
  }

  bool vector_graph::init_scene_graph()
  {
    debug::log::get(debug::logINFO) << "Initializing " << scene_graph_id << std::endl;

    return true;
  }

  mesh_id vector_graph::create_node(std::shared_ptr<resource::mesh> mesh)
  {
    debug::log::get(debug::logINFO) << "Adding a mesh to the scene graph" << std::endl;

    meshes.push_back(mesh);
    render_queue.push_back(mesh);

    debug::log::get(debug::logINDENT) << "Number of meshes: " << meshes.size() << std::endl;

    return meshes.size() - 1;
  }
}
