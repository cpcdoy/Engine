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
    material.albedo_tex = tex;
  }

  GLuint gl_mesh::get_texture()
  {
    return material.albedo_tex;
  }

  void gl_mesh::set_normal_texture(GLuint tex)
  {
    material.normal_tex = tex;
  }

  GLuint gl_mesh::get_normal_texture()
  {
    return material.normal_tex;
  }

  void gl_mesh::set_roughness_texture(GLuint tex)
  {
    material.roughness_tex = tex;
  }

  GLuint gl_mesh::get_roughness_texture()
  {
    return material.roughness_tex;
  }

  void gl_mesh::set_metalness_texture(GLuint tex)
  {
    material.metalness_tex = tex;
  }

  GLuint gl_mesh::get_metalness_texture()
  {
    return material.metalness_tex;
  }
}
