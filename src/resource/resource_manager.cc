#include "resource_manager.hh"

namespace resource
{
  resource_manager::resource_manager(resource_manager_flags f, std::shared_ptr<render_backend::render_manager> rb)
  {
    (void)f;
    managees.push_back(std::make_shared<model_loader>());
    managees.push_back(std::make_shared<obj_loader>());

    this->rb = rb;
  }

  resource_manager::resource_manager(std::shared_ptr<render_backend::render_manager> rb)
    : resource_manager(resource_manager_flags::ALL, rb)
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
        meshes.push_back(rb->generate_compatible_mesh((*i)->generate_mesh()));
        //auto m = std::static_pointer_cast<resource::gl_mesh>(meshes.back());
        //std::cout << "vao " << m->get_vao();
        return meshes.back();
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "Resource loading of " << path << " failed"
                                    << std::endl;
    return nullptr;
  }

  std::vector<std::shared_ptr<mesh>> resource_manager::get_meshes()
  {
    return meshes;
  }
}
