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

        rb->update_renderer();

        auto v = sm->get_render_queue();
        for (auto i = v.begin(); i != v.end(); i++)
          rb->render((*i));

        ui->update_ui();
      }
      debug::log::get(debug::logINDENT, 5) << "Finished" << std::endl;
    }
}
