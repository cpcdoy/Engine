#include "ui_manager.hh"

namespace ui
{
  ui_manager::ui_manager()
  {
    uis.push_back(std::make_shared<glfw_ui>());
  }

  ui_manager::~ui_manager()
  {
    uis.clear();
  }

  bool ui_manager::init_ui()
  {
    debug::log::get(debug::logINFO) << "Loading UI system" << std::endl;

    for (auto i = uis.begin(); i != uis.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying UI system \"" << (*i)->get_base_ui_id()
                                           << "\"" << std::endl;
      if ((*i)->init_base_ui())
        return true;
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "UI system loading failed" 
                                     << std::endl;
    return false;
  }
}
