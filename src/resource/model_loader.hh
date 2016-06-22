#pragma once

# include <memory>

# include "../debug/log.hh"
# include "mesh.hh"

namespace resource
{
  class model_loader
  {
    public:
      model_loader()
      {
      }

      virtual ~model_loader()
      {
      }

      virtual bool load(const char* path)
      {
        (void)path;
        return false;
      }

      virtual std::shared_ptr<mesh> generate_mesh()
      {
        return std::make_shared<mesh>();
      }

      virtual std::string get_loader_id()
      {
        return loader_id;
      }

    protected:
      std::string loader_id = "virtual empty loader";
  };
}
