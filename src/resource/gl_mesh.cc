#include "gl_mesh.hh"

namespace resource
{

  gl_mesh::gl_mesh()
    : mesh()
  {
  }

  gl_mesh::gl_mesh(std::shared_ptr<mesh> m)
  {
    lods[lod].vertices = m->get_vertices();
    lods[lod].uvs = m->get_uvs();
    lods[lod].normals = m->get_normals();
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

  void gl_mesh::set_texture(GLuint tex)
  {
    this->albedo_tex = tex;
  }

  GLuint gl_mesh::get_texture()
  {
    return albedo_tex;
  }

  void gl_mesh::set_normal_texture(GLuint tex)
  {
    this->albedo_tex = tex;
  }

  GLuint gl_mesh::get_normal_texture()
  {
    return albedo_tex;
  }
}
