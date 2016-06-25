#include "resource_manager.hh"

namespace resource
{
  resource_manager::resource_manager(resource_manager_flags f)
  {
    (void)f;
    managees.push_back(std::make_shared<model_loader>());
    managees.push_back(std::make_shared<obj_loader>());
  }

  resource_manager::resource_manager()
    : resource_manager(resource_manager_flags::ALL)
  {
  }

  resource_manager::~resource_manager()
  {
    managees.clear();
    meshes.clear();
  }

  std::shared_ptr<mesh> resource_manager::load(std::string path)
  {
    debug::log::get(debug::logINFO) << "Loading resource " << path << std::endl;

    for (auto i = managees.begin(); i != managees.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying loader \"" << (*i)->get_loader_id()
                                           << "\"" << std::endl;

      if ((*i)->load(path.c_str()))
      {
        meshes.push_back((*i)->generate_mesh());
        return meshes.back();
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "Resource loading of " << path << " failed"
                                    << std::endl;
    return nullptr;
  }
}
