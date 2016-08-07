#include "glfw_ui.hh"

namespace ui
{
  glfw_ui::glfw_ui()
    : base_ui()
  {
    base_ui_id = "glfw_ui";
    major_backend_version = minor_backend_version = 3;
  }

  glfw_ui::~glfw_ui()
  {
    debug::log::get(debug::logINFO) << "Cleaning the GLFW context" << std::endl;
    glfwDestroyWindow(window);
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
    
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);

    debug::log::get(debug::logINDENT, 5) << "GLFW version " << major << "." << minor << "." << revision << std::endl;
    debug::log::get(debug::logINDENT, 5) << "OpenGL version " << major_backend_version << "." << minor_backend_version << std::endl;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, true);

    tex_streamer_fake_window = glfwCreateWindow(1, 1, "fake", nullptr, nullptr);
    window = glfwCreateWindow(w, h, title.c_str(), NULL, tex_streamer_fake_window);

    if (!window)
    {
      debug::log::get(debug::logERROR) << "Failed to open a GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSwapInterval(1);

    return true;
  }

  bool glfw_ui::satisfies_running_condition()
  {
    return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
  }

  void glfw_ui::update_ui()
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GLFWwindow* glfw_ui::get_window()
  {
    return window;
  }

  GLFWwindow* glfw_ui::get_fake_window()
  {
    return tex_streamer_fake_window;
  }
}
