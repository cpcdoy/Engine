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

  void
    render_manager::set_compatible_texture(std::shared_ptr<resource::mesh>& mesh, unsigned char* tex, int w, int h, texture_kind k)
    {
      current_managee->set_compatible_texture(mesh, tex, w, h, k);
    }

  void render_manager::set_compatible_texture(std::shared_ptr<resource::mesh>& mesh, std::string path, texture_kind k)
  {
    current_managee->set_compatible_texture(mesh, path, k);
  }

  void render_manager::init_render_backend(std::shared_ptr<scene::scene_manager> sm)
  {
    current_managee->init_render_backend(sm);
  }

  void render_manager::render(long rq_size)
  {
    current_managee->render(rq_size);
  }

  void render_manager::set_managers(std::shared_ptr<ui::ui_manager> ui, std::shared_ptr<resource::resource_manager> rm)
  {
    current_managee->set_managers(ui, rm);
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

  void render_manager::set_clear_color(float r, float g, float b)
  {
    current_managee->set_clear_color(r, g, b);
  }
}
