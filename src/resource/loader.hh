#pragma once

# include "../debug/log.hh"

namespace resource
{
  class loader
  {
    public:
      virtual void load(const char* path) = 0;
  };
}
