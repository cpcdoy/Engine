#pragma once

# include "engine.hh"

namespace engine
{
  template<typename F>
    void engine::run(F f)
    {
      debug::log::get(debug::logINFO) << "Running the engine's main loop" << std::endl;
      while (ui->ui_satisfies_running_condition())
      {
        f();
        ui->update_ui();
      }
      debug::log::get(debug::logINDENT) << "Finished" << std::endl;
    }
}
