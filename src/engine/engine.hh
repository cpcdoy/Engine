#pragma once

# include <algorithm>
# include <memory>

# include "../resource/resource_manager.hh"
# include "../render_backend/render_manager.hh"
# include "../ui/ui_manager.hh"
# include "../debug/debug_manager.hh"
# include "../scene/scene_manager.hh"

# include "../render_backend/backend_plugins.hh"

typedef std::shared_ptr<resource::mesh> mesh;
typedef std::shared_ptr<scene::camera> camera;

namespace engine
{
  class engine
  {
    public:
      engine();
      ~engine();

      bool set_engine_render_context(
          render_backend::render_plugins r = render_backend::render_plugins::OpenGL,
          int major = 3, int minor = 3);

      void set_window_properties(int w, int h, std::string title);

      mesh load_mesh(std::string p);
      void add_mesh_lod(std::string path, mesh& mesh, int dist, int lod);
      void load_texture(std::string p, mesh& m, texture_kind k = texture_kind::ALBEDO);
      std::shared_ptr<scene::camera> create_camera();
      void set_current_camera(camera cam);
      void set_clear_color(float r, float g, float b);

      bool init();

      template<typename... Functions>
        void run(Functions... fs);

    private:
      std::shared_ptr<render_backend::render_manager> rb;
      std::shared_ptr<resource::resource_manager> rm;
      std::shared_ptr<ui::ui_manager> ui;
      std::shared_ptr<debug::debug_manager> dm;
      std::shared_ptr<scene::scene_manager> sm;

      render_backend::render_plugins r;
      int major, minor;

      int w, h;
      std::string title;
  };
}

# include "engine.hxx"
