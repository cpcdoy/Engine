#pragma once

# include "engine.hh"

namespace engine
{
  template<typename... Functions>
    void engine::run(Functions... fs)
    {
      debug::log::get(debug::logINFO) << "Running the engine's main loop" << std::endl;

      rb->init_render_backend(sm);
      while (ui->ui_satisfies_running_condition())
      {
        for (const auto& f : {fs...})
          f();

        auto cam = sm->get_current_camera();
        auto& rq = sm->get_render_queue();

        auto rq_size = sm->compute_view_frustum_culling(rq, cam);
        sm->set_render_queue_size(rq_size);

        rb->update_renderer();
        rb->render(rq_size);

        ui->update_ui();
      }
      event::channel::broadcast(event::engine_stop_event {});
    }
}
