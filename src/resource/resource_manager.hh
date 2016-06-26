#pragma once

# include <vector>
# include <memory>

# include "../debug/log.hh"
# include "entity.hh"
# include "model_loader.hh"
# include "loaders.hh"
# include "../base/manager.hh"
# include "../render_backend/render_manager.hh"

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

      std::shared_ptr<mesh> load(std::string path);

      std::vector<std::shared_ptr<mesh>> get_meshes();

    private:
      std::vector<std::shared_ptr<mesh>> meshes;
      std::shared_ptr<render_backend::render_manager> rb;
  };
}
