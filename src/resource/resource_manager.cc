#include "resource_manager.hh"

namespace resource
{
  resource_manager::resource_manager(resource_manager_flags f)
  {
    (void)f;
    loaders.push_back(std::make_shared<obj_loader>());
  }

  resource_manager::resource_manager()
    : resource_manager(resource_manager_flags::ALL)
  {
  }

  resource_manager::~resource_manager()
  {
    loaders.clear();
    meshes.clear();
  }

  std::shared_ptr<mesh> resource_manager::load(std::string path)
  {
    debug::log::get(debug::logINFO) << "Loading resource " << path << std::endl;

    size_t i;
    for (i = 0; i < loaders.size(); i++)
      if (loaders[i]->load(path.c_str()))
      {
        meshes.push_back(loaders[i]->generate_mesh());
        return meshes.back();
      }

    return nullptr;
  }
}
