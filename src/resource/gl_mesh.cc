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
    auto t = material.albedo_streamed_tex;
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_normal_texture(GLuint tex)
  {
    material.normal_tex = tex;
  }

  GLuint gl_mesh::get_normal_texture()
  {
    auto t = material.normal_streamed_tex;
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_roughness_texture(GLuint tex)
  {
    material.roughness_tex = tex;
  }

  GLuint gl_mesh::get_roughness_texture()
  {
    auto t = material.roughness_streamed_tex;
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_metalness_texture(GLuint tex)
  {
    material.metalness_tex = tex;
  }

  GLuint gl_mesh::get_metalness_texture()
  {
    auto t = material.metalness_streamed_tex;
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_ao_texture(GLuint tex)
  {
    material.ao_tex = tex;
  }

  GLuint gl_mesh::get_ao_texture()
  {
    auto t = material.ao_streamed_tex;
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_streamed_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.albedo_streamed_tex = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_texture()
  {
    return material.albedo_streamed_tex;
  }

  void gl_mesh::set_streamed_normal_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.normal_streamed_tex = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_normal_texture()
  {
    return material.normal_streamed_tex;
  }

  void gl_mesh::set_streamed_roughness_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.roughness_streamed_tex = tex;;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_roughness_texture()
  {
    return material.roughness_streamed_tex;
  }

  void gl_mesh::set_streamed_metalness_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.metalness_streamed_tex = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_metalness_texture()
  {
    return material.metalness_streamed_tex;
  }

  void gl_mesh::set_streamed_ao_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.ao_streamed_tex = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_ao_texture()
  {
    return material.ao_streamed_tex;
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
