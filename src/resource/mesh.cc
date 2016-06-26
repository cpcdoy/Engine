#include "mesh.hh"

namespace resource
{

  mesh::mesh()
  {
    lod = 0;
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
}
