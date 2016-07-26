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
    this->vaos.push_back(vao);
  }

  GLuint gl_mesh::get_vao()
  {
    return vaos[lod];
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

  void gl_mesh::set_ao_texture(GLuint tex)
  {
    material.ao_tex = tex;
  }

  GLuint gl_mesh::get_ao_texture()
  {
    return material.ao_tex;
  }

  void gl_mesh::add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh)
  {
    mesh_resource m_r(dist, lod);
    lods.push_back(m_r);

    set_vertices(mesh->get_vertices(), lod);
    set_uvs(mesh->get_uvs(), lod);
    set_normals(mesh->get_normals(), lod);

    std::shared_ptr<resource::gl_mesh> gl_mesh = std::static_pointer_cast<resource::gl_mesh>(mesh);
    set_vao(gl_mesh->get_vao());
  }
}
