#include "scene_manager.hh"

namespace scene
{
  scene_manager::scene_manager(std::shared_ptr<resource::resource_manager> rm)
  {
    managees.push_back(std::make_shared<vector_graph>());

    this->rm = rm;
  }

  scene_manager::~scene_manager()
  {
    managees.clear();
  }

  bool scene_manager::init_scene_graph()
  {
    debug::log::get(debug::logINFO) << "Initializing the scene graph" << std::endl;

    for (auto i = managees.begin(); i != managees.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying scene graph \"" << (*i)->get_scene_graph_id()
                                           << "\"" << std::endl;

      if ((*i)->init_scene_graph())
      {
        current_managee = (*i);
        return true;
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "Scene graph initialization failed"
                                     << std::endl;
    debug::log::get(debug::logINDENT) << "Check that at least one scene graph is plugged-in"
                                     << std::endl;
    return false;
  }

  mesh_id scene_manager::create_node(std::shared_ptr<resource::mesh> mesh)
  {
    return current_managee->create_node(mesh);
  }

  std::vector<std::shared_ptr<resource::mesh>> scene_manager::get_render_queue()
  {
    return current_managee->get_render_queue();
  }
}
