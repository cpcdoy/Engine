#include "engine.hh"

namespace engine
{
  engine::engine()
    : 
    rm(std::make_shared<resource::resource_manager>()),
    rb(std::make_shared<render_backend::render_manager>()),
    ui(std::make_shared<ui::ui_manager>()),
    dm(std::make_shared<debug::debug_manager>()),
      r(render_backend::render_plugins::OpenGL)
  {
    major = minor = 3;
    w = 800;
    h = 600;
    title = "Engine";
  }

  engine::~engine()
  {
  }

  bool engine::set_engine_render_context(render_backend::render_plugins r, int major, int minor)
  {
    debug::log::get(debug::logINFO) << "Explicitly setting the render context" << std::endl;
    if (r < render_backend::render_plugins::ALL)
      this->r = r;
    else
    {
      debug::log::get(debug::logERROR) << "Explicitly given render backend does not exist" << std::endl;
      return false;
    }
    this->major = major;
    this->minor = minor;

    return true;
  }

  void engine::set_window_properties(int w, int h, std::string title)
  {
    debug::log::get(debug::logINFO) << "Setting the windows' properties" << std::endl;
    debug::log::get(debug::logINDENT, 5) << "Size: " << w << "*" << h << std::endl;
    debug::log::get(debug::logREINDENT) << "Title: " << title << std::endl;

    this->w = w;
    this->h = h;
    this->title = title;
  }

  bool engine::init()
  {
    ui->init_ui();
    ui->set_backend_context_version(major, minor);
    ui->create_window(h, w, title);

    rb->init_render_backend();

    return true;
  }

  void engine::load_mesh(std::string p)
  {
    rm->load(p);
  }
}
