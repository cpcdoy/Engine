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
        auto cam_pos = cam->get_camera_position();
        for (auto m : sm->get_render_queue())
        {
          if (cam->point_in_view(m->get_pos()))
          {
            m->compute_current_lod(cam_pos);
            std::cout << "in view" << std::endl;
          }
          else
          {
            std::cout << "not in vew" << std::endl;
            //sm->get_render_queue().erase(std::remove(sm->get_render_queue().begin(), sm->get_render_queue().end(), m), sm->get_render_queue().end());
          }
        }
        std::cout << "------------" << std::endl;

        rb->update_renderer();
        rb->render();

        ui->update_ui();
      }
      debug::log::get(debug::logINDENT, 5) << "Finished" << std::endl;
    }
}
