#include "soil_loader.hh"
#include <iostream>

namespace resource
{
  soil_loader::soil_loader()
    : texture_loader()
  {
    loader_id = "soil_loader";
  }

  bool soil_loader::load(const char* path)
  {
    tex = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

    return tex != nullptr;
  }

  unsigned char* soil_loader::get_generated_texture()
  {
    return tex;
  }
}
