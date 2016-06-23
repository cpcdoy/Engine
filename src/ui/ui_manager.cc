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
      {
        current_ui = *i;
        return true;
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "UI system loading failed" 
                                     << std::endl;
    return false;
  }

  bool ui_manager::create_window(int h, int w, std::string title)
  {
    return current_ui->create_window(h, w, title);
  }

  bool ui_manager::ui_satisfies_running_condition()
  {
    return current_ui->satisfies_running_condition();
  }

  void ui_manager::update_ui()
  {
    current_ui->update_ui();
  }
}
