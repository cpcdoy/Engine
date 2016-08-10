#pragma once

# include <vector>
# include <algorithm>

# include "../debug/log.hh"
# include "texture_loader.hh"
# include "../include/SOIL/src/SOIL.h"

namespace resource
{
  class soil_loader : public texture_loader
  {
    public:
      soil_loader();
      virtual ~soil_loader() override;

      virtual bool load(const char* path) override;
      virtual unsigned char* get_generated_texture() override;
      virtual void clean() override;
    private:
      std::vector<unsigned char*> texs;
  };
}
