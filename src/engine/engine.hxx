#pragma once

# include "engine.hh"

namespace engine
{
  template<typename... Functions>
    void engine::run(Functions... fs)
    {
      debug::log::get(debug::logINFO) << "Running the engine's main loop" << std::endl;

      rb->batch(sm);
      while (ui->ui_satisfies_running_condition())
      {
        for (const auto& f : {fs...})
          f();

        rb->update_renderer();
        rb->render();

        ui->update_ui();
      }
      debug::log::get(debug::logINDENT, 5) << "Finished" << std::endl;
    }
}
