#include "ui_manager.hh"

namespace ui
{
  ui_manager::ui_manager()
  {
    managees.push_back(std::make_shared<glfw_ui>());
  }

  ui_manager::~ui_manager()
  {
    managees.clear();
  }

  bool ui_manager::init_ui()
  {
    debug::log::get(debug::logINFO) << "Loading UI system" << std::endl;

    for (auto i = managees.begin(); i != managees.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying UI system \"" << (*i)->get_base_ui_id()
                                           << "\"" << std::endl;
      if ((*i)->init_base_ui())
      {
        current_managee = *i;
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
    return current_managee->create_window(h, w, title);
  }

  bool ui_manager::ui_satisfies_running_condition()
  {
    return current_managee->satisfies_running_condition();
  }

  void ui_manager::update_ui()
  {
    current_managee->update_ui();
  }

  void ui_manager::set_backend_context_version(int major, int minor)
  {
    current_managee->set_backend_context_version(major, minor);
  }

  std::shared_ptr<base_ui> ui_manager::get_ui()
  {
    return current_managee;
  }
}
