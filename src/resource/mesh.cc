#include "mesh.hh"
# include <iostream>

namespace resource
{

  mesh::mesh()
  {
    lod = 0;
    mesh_resource m_r(20, 0);

    lods.push_back(m_r);

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

  void mesh::set_vertices(std::vector<glm::vec3> v, int lod)

  {
    lods[lod].vertices = v;
  }

  void mesh::set_uvs(std::vector<glm::vec2> u, int lod)
  {
    lods[lod].uvs = u;
  }

  void mesh::set_normals(std::vector<glm::vec3> n, int lod)
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
    this->pos = pos;
    model = glm::translate(model, pos);
  }

  glm::vec3 mesh::get_pos()
  {
    return pos;
  }

  void mesh::set_scale(glm::vec3 scale)
  {
    model = glm::scale(model, scale);
  }

  void mesh::set_rotation(float ang, glm::vec3 axis)
  {
    model = glm::rotate(model, ang, axis);
  }

  void mesh::compute_current_lod(glm::vec3 cam_pos)
  {
    glm::vec3 mesh_pos = cam_pos - glm::vec3(model[3]);
    float cam_dist = glm::dot(mesh_pos, mesh_pos);

    auto i = lods.begin();
    for (; i != lods.end(); i++)
      if (cam_dist <= (*i).dist)
        break;

    if (i == lods.end())
      this->lod = lods.back().level;
    else
      this->lod = (*i).level;
  }

  void mesh::add_lod(int, int, std::shared_ptr<mesh>&)
  {
  }

  void mesh::set_lod_distance(int lod, int dist)
  {
    lods[lod].dist = dist;
  }

  int mesh::get_current_lod()
  {
    return lod;
  }

  glm::mat4 mesh::get_model()
  {
    return model;
  }

  void mesh::set_aabb(glm::vec3 center, glm::vec3 ext)
  {
    lods[lod].aabb = aabb(center, ext);
  }

  struct aabb mesh::get_aabb()
  {
    return lods[lod].aabb;
  }
}
