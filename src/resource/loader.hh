#pragma once

# include <memory>

# include "../debug/log.hh"
# include "mesh.hh"

namespace resource
{
  class loader
  {
    public:
      loader()
      {
      }

      virtual ~loader()
      {
      }

      virtual bool load(const char* path)
      {
        (void)path;
        return true;
      }

      virtual std::shared_ptr<mesh> generate_mesh()
      {
        return std::make_shared<mesh>();
      }

  };
}
