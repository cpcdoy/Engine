#pragma once

//This header's order is important to avoid type conflicts
# include <GL/glew.h>
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
      virtual void update_ui() override;
      virtual bool satisfies_running_condition() override;

    protected:
      GLFWwindow* window;
  };
}
