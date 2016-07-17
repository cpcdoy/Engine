#pragma once

# include "texture_loader.hh"
# include "../include/SOIL/src/SOIL.h"

namespace resource
{
  class soil_loader : public texture_loader
  {
    public:
      soil_loader();

      virtual bool load(const char* path) override;
      virtual unsigned char* get_generated_texture() override;
  };
}
