#include "opengl_backend.hh"

namespace render_backend
{
  opengl_backend::opengl_backend()
  {
    backend_id = "OpenGL Backend";
  }

  opengl_backend::~opengl_backend()
  {
  }

  bool opengl_backend::init_backend()
  {
    glewExperimental = true;
    if (glewInit() != GLEW_OK) 
    {
      debug::log::get(debug::logERROR) << "Failed to initialize GLEW" << std::endl;
      debug::log::get(debug::logINDENT) << "You need to call the ui_manager, initialize it and create a window before" << std::endl;
      return false;
    }
    return true;
  }
}
