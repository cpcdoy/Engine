#include "mesh.hh"
# include <iostream>

namespace resource
{

  mesh::mesh()
  {
    lod = 0;

    model = glm::mat4(1.0);
  }

  mesh::~mesh()
  {
  }

  void mesh::set_vertices(std::vector<glm::vec3> v)
  {
    vertices = v;
  }

  void mesh::set_uvs(std::vector<glm::vec2> u)
  {
    uvs = u;
  }

  void mesh::set_normals(std::vector<glm::vec3> n)
  {
    normals = n;
  }

  std::vector<glm::vec3> mesh::get_vertices()
  {
    return vertices;
  }

  std::vector<glm::vec2> mesh::get_uvs()
  {
    return uvs;
  }

  std::vector<glm::vec3> mesh::get_normals()
  {
    return normals;
  }

  void mesh::set_pos(glm::vec3 pos)
  {
    model = glm::translate(model, pos);
  }

  void mesh::set_scale(glm::vec3 scale)
  {
    model = glm::scale(model, scale);
  }

  glm::mat4 mesh::get_model()
  {
    return model;
  }
}
