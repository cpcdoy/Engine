#include "mesh.hh"

namespace resource
{

  mesh::mesh()
  {
    lod = 0;
    lods.push_back(mesh_resource());

    model = glm::mat4(1.0);
  }

  mesh::~mesh()
  {
  }

  void mesh::set_vertices(std::vector<glm::vec3> v)
  {
    lods[lod].vertices = v;
  }

  void mesh::set_uvs(std::vector<glm::vec2> u)
  {
    lods[lod].uvs = u;
  }

  void mesh::set_normals(std::vector<glm::vec3> n)
  {
    lods[lod].normals = n;
  }

  std::vector<glm::vec3> mesh::get_vertices()
  {
    return lods[lod].vertices;
  }

  std::vector<glm::vec2> mesh::get_uvs()
  {
    return lods[lod].uvs;
  }

  std::vector<glm::vec3> mesh::get_normals()
  {
    return lods[lod].normals;
  }

  void mesh::set_pos(glm::vec3 pos)
  {
    model = glm::translate(model, pos);
  }

  void mesh::set_scale(glm::vec3 scale)
  {
    model = glm::scale(model, scale);
  }

  void mesh::set_current_lod(glm::vec3 cam_pos)
  {
    glm::vec3 mesh_pos = cam_pos - glm::vec3(model[3]);
    float cam_dist = glm::dot(mesh_pos, mesh_pos);

    auto i = lods.begin();
    for (; i != lods.end(); i++)
      if (cam_dist < (*i).dist)
        break;

    this->lod = i->level;
  }

  int mesh::get_current_log()
  {
    return lod;
  }

  glm::mat4 mesh::get_model()
  {
    return model;
  }
}
