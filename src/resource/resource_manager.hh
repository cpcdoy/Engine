#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <vector>
# include <memory>

# include "texture_kind.hh"
# include "../render_backend/render_manager.hh"
# include "mesh.hh"
# include "loaders.hh"
# include "../base/manager.hh"
# include "../event/events.hh"

namespace render_backend
{
  class render_manager;
}

namespace resource
{
  enum resource_manager_flags
  {
    LOAD_OBJ,
    ALL,
  };

  class resource_manager : public base::base_manager<model_loader>
  {
    public:
      resource_manager(std::shared_ptr<render_backend::render_manager> rb);
      resource_manager(resource_manager_flags f, std::shared_ptr<render_backend::render_manager> rb);
      ~resource_manager();

      std::shared_ptr<mesh> load_mesh(std::string path);
      void add_mesh_lod(std::string path, std::shared_ptr<mesh>& mesh, int dist, int lod);
      void load_texture(std::string path, std::shared_ptr<mesh>& mesh, texture_kind k = texture_kind::ALBEDO);
      unsigned char* load_texture(std::string path);

      std::vector<std::shared_ptr<mesh>> get_meshes();

      void operator()(const event::engine_stop_event&)
      {
        debug::log::get(debug::logINFO) << "Resource manager shutting down..." << std::endl;
      }

    private:
      std::vector<std::shared_ptr<mesh>> meshes;
      std::shared_ptr<render_backend::render_manager> rb;

      std::vector<std::shared_ptr<texture_loader>> tex_loaders;
  };
}
