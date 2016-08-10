#include "soil_loader.hh"

namespace resource
{
  soil_loader::soil_loader()
    : texture_loader()
  {
    loader_id = "soil_loader";
  }

  soil_loader::~soil_loader()
  {
    debug::log::get(debug::log_level::logINFO) << "Cleaning the " << loader_id << "'s resources" << std::endl;
    std::for_each(texs.begin(), texs.end(), &SOIL_free_image_data);
  }

  bool soil_loader::load(const char* path)
  {
    tex = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_RGBA);
    texs.push_back(tex);

    if (!tex)
    {
      std::string str = std::string(path);
      debug::log::get(debug::log_level::logERROR) << SOIL_last_result() << std::endl;
      if (str.find(".jpg") != std::string::npos)
        debug::log::get(debug::log_level::logINDENT, 5) << "Try to convert this JPEG image to a non-progressive one" << std::endl;
    }

    return tex != nullptr;
  }

  unsigned char* soil_loader::get_generated_texture()
  {
    return tex;
  }

  void soil_loader::clean()
  {
    SOIL_free_image_data(tex);
  }
}
