#include "resource_manager.hh"

namespace resource
{
  resource_manager::resource_manager(resource_manager_flags f, std::shared_ptr<render_backend::render_manager> rb)
  {
    (void)f;
    managees.push_back(std::make_shared<obj_loader>());

    tex_loaders.push_back(std::make_shared<soil_loader>());

    this->rb = rb;
  }

  resource_manager::resource_manager(std::shared_ptr<render_backend::render_manager> rb)
    : resource_manager(resource_manager_flags::ALL, rb)
  {
  }

  resource_manager::~resource_manager()
  {
    managees.clear();
    tex_loaders.clear();

    meshes.clear();
  }

  std::shared_ptr<mesh> resource_manager::load_mesh(std::string path)
  {
    debug::log::get(debug::logINFO) << "Loading model resource " << path << std::endl;

    for (auto i = managees.begin(); i != managees.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying loader \"" << (*i)->get_loader_id()
                                           << "\"" << std::endl;

      if ((*i)->load(path.c_str()))
      {
        meshes.push_back(rb->generate_compatible_mesh((*i)->generate_mesh()));
        return meshes.back();
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "Resource loading of " << path << " failed"
                                    << std::endl;
    return nullptr;
  }

  void resource_manager::load_texture(std::string path, std::shared_ptr<mesh>& mesh)
  {
    debug::log::get(debug::logINFO) << "Loading texture resource " << path << std::endl;

    for (auto i = tex_loaders.begin(); i != tex_loaders.end(); i++)
    {
      debug::log::get(debug::logINDENT, 5) << "Trying loader \"" << (*i)->get_loader_id()
                                           << "\"" << std::endl;

      if ((*i)->load(path.c_str()))
      {
        rb->set_compatible_texture(mesh, (*i)->get_generated_texture(), (*i)->get_width(), (*i)->get_height());
        return;
      }
      debug::log::get(debug::logINDENT, 10) << "FAIL" << std::endl;
    }

    debug::log::get(debug::logERROR) << "Resource loading of " << path << " failed"
                                    << std::endl;
  }

  std::vector<std::shared_ptr<mesh>> resource_manager::get_meshes()
  {
    return meshes;
  }
}
