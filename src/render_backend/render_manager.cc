#include "render_manager.hh"

namespace render_backend
{
  render_manager::render_manager()
  {
    managees.push_back(std::make_shared<opengl_backend>());
  }

  render_manager::~render_manager()
  {
  }

  bool render_manager::init_render_backend(int w, int h)
  {
    debug::log::get(debug::logINFO) << "Loading render backend" << std::endl;

    for (auto i = managees.begin(); i != managees.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying render backend \"" << (*i)->get_backend_id()
                                           << "\"" << std::endl;
      if ((*i)->init_backend(w, h))
      {
        current_managee = *i;
        return true;
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "Render backend loading failed" 
                                     << std::endl;
    return false;
  }

  std::shared_ptr<resource::mesh>
    render_manager::generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh)
    {
      return current_managee->generate_compatible_mesh(mesh);
    }

  void render_manager::batch(std::shared_ptr<scene::scene_manager> sm)
  {
    current_managee->batch(sm);
  }

  void render_manager::render()
  {
    current_managee->render();
  }

  void render_manager::set_ui_manager(std::shared_ptr<ui::ui_manager> ui)
  {
    current_managee->set_ui_manager(ui);
  }

  void render_manager::set_camera(std::shared_ptr<scene::camera> cam)
  {
    current_managee->set_camera(cam);
  }

  void render_manager::update_renderer()
  {
    current_managee->update_renderer();
  }

  void render_manager::add_state(std::string s, long r)
  {
    current_managee->add_state(s, r);
  }
}
