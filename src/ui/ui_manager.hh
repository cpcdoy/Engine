#pragma once

# include <memory>
# include <vector>

# include "base_ui.hh"
# include "uis.hh"

namespace ui
{
  class ui_manager
  {
    public:
      ui_manager();
      ~ui_manager();

      bool init_ui();
      bool create_window(int h, int w, std::string title);
      bool ui_satisfies_running_condition();
      void update_ui();

    private:
      std::vector<std::shared_ptr<base_ui>> uis;
      std::shared_ptr<base_ui> current_ui;
  };
}
