#pragma once

# include <vector>
# include <memory>

# include "../debug/log.hh"
# include "entity.hh"
# include "model_loader.hh"
# include "loaders.hh"

namespace resource
{
  enum resource_manager_flags
  {
    LOAD_OBJ,
    ALL,
  };

  class resource_manager
  {
    public:
      resource_manager();
      resource_manager(resource_manager_flags f);
      ~resource_manager();

      std::shared_ptr<mesh> load(std::string path);

    private:
      std::vector<std::shared_ptr<model_loader>> loaders;
      std::vector<std::shared_ptr<mesh>> meshes;
  };
}
