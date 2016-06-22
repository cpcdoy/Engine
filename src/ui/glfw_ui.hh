#pragma once

# include <GLFW/glfw3.h>

# include "base_ui.hh"
# include "../debug/log.hh"

namespace ui
{
  class glfw_ui : public base_ui
  {
    public:
      glfw_ui();
      ~glfw_ui();

      virtual bool init_base_ui() override;
      virtual bool create_window(int h, int w, std::string title) override;

    protected:
      GLFWwindow* window;
  };
}
