#include "glfw_ui.hh"

namespace ui
{
  glfw_ui::glfw_ui()
    : base_ui()
  {
    base_ui_id = "glfw_ui";
  }

  glfw_ui::~glfw_ui()
  {
    debug::log::get(debug::logINFO) << "Cleaning the GLFW context" << std::endl;
    glfwTerminate();
  }
  
  bool glfw_ui::init_base_ui()
  {
    debug::log::get(debug::logINFO) << "Initializing the GLFW UI" << std::endl;
    if (!glfwInit())
    {
      debug::log::get(debug::logERROR) << "Failed to init " << base_ui_id << std::endl;
      return false;
    }
    return true;
  }

  bool glfw_ui::create_window(int h, int w, std::string title)
  {
    debug::log::get(debug::logINFO) << "Creating a GLFW window" << std::endl;
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
    if (!window)
    {
      debug::log::get(debug::logERROR) << "Failed to open a GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    return true;
  }
}
