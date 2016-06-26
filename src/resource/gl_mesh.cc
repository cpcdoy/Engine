#include "gl_mesh.hh"

namespace resource
{

  gl_mesh::gl_mesh()
    : mesh()
  {
  }

  gl_mesh::gl_mesh(std::shared_ptr<mesh> m)
  {
    vertices = m->get_vertices();
    uvs = m->get_uvs();
    normals = m->get_normals();
  }

  gl_mesh::~gl_mesh()
  {
  }

  void gl_mesh::set_vao(GLuint vao)
  {
    this->vao = vao;
  }

  GLuint gl_mesh::get_vao()
  {
    return vao;
  }
}
