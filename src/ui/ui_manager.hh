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

    private:
      std::vector<std::shared_ptr<base_ui>> uis;
  };
}
