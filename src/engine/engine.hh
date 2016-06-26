#pragma once

# include "../resource/resource_manager.hh"
# include "../render_backend/render_manager.hh"
# include "../ui/ui_manager.hh"
# include "../debug/debug_manager.hh"

# include "../render_backend/backend_plugins.hh"

namespace engine
{
  class engine
  {
    public:
      engine();
      ~engine();

      bool set_engine_render_context(
          render_backend::render_plugins r=render_backend::render_plugins::OpenGL,
          int major=3, int minor=3);

      void set_window_properties(int w, int h, std::string title);
      void load_mesh(std::string p);

      bool init();

      template<typename F>
        void run(F f);

    private:
      std::shared_ptr<render_backend::render_manager> rb;
      std::shared_ptr<resource::resource_manager> rm;
      std::shared_ptr<ui::ui_manager> ui;
      std::shared_ptr<debug::debug_manager> dm;

      render_backend::render_plugins r;
      int major, minor;

      int w, h;
      std::string title;
  };
}

# include "engine.hxx"
