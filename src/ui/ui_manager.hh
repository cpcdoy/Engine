#pragma once

# include <memory>
# include <vector>

# include "../base/manager.hh"
# include "base_ui.hh"
# include "uis.hh"

namespace ui
{
  class ui_manager : public base::base_manager<base_ui>
  {
    public:
      ui_manager();
      ~ui_manager();

      bool init_ui();
      bool create_window(int h, int w, std::string title);
      bool ui_satisfies_running_condition();
      void update_ui();
      void set_backend_context_version(int major, int minor);
  };
}
