#include "scene_manager.hh"

namespace scene
{
  scene_manager::scene_manager(std::shared_ptr<resource::resource_manager> rm,
                              std::shared_ptr<ui::ui_manager> ui)
  {
    managees.push_back(std::make_shared<vector_graph>());

    this->rm = rm;
    this->ui = ui;
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
        current_managee->set_ui_manager(ui);
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

  std::shared_ptr<resource::mesh> scene_manager::create_node(std::shared_ptr<resource::mesh> mesh)
  {
    return current_managee->create_node(mesh);
  }

  std::shared_ptr<scene::camera> scene_manager::create_camera()
  {
    return current_managee->create_camera();
  }

  void scene_manager::set_current_camera(std::shared_ptr<scene::camera> cam)
  {
    current_managee->set_current_camera(cam);
  }

  std::shared_ptr<scene::camera> scene_manager::get_current_camera()
  {
    return current_managee->get_current_camera();
  }

  std::vector<std::shared_ptr<resource::mesh>> scene_manager::get_render_queue()
  {
    return current_managee->get_render_queue();
  }
}
