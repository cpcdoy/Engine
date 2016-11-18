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
    lods[lod].tangents = m->get_tangents();
    lods[lod].bitangents = m->get_bitangents();
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
    auto t = material.streamed_texs[texture_kind::ALBEDO];
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_normal_texture(GLuint tex)
  {
    material.normal_tex = tex;
  }

  GLuint gl_mesh::get_normal_texture()
  {
    auto t = material.streamed_texs[texture_kind::NORMAL];
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_roughness_texture(GLuint tex)
  {
    material.roughness_tex = tex;
  }

  GLuint gl_mesh::get_roughness_texture()
  {
    auto t = material.streamed_texs[texture_kind::ROUGHNESS];
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_metalness_texture(GLuint tex)
  {
    material.metalness_tex = tex;
  }

  GLuint gl_mesh::get_metalness_texture()
  {
    auto t = material.streamed_texs[texture_kind::METALNESS];
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_ao_texture(GLuint tex)
  {
    material.ao_tex = tex;
  }

  GLuint gl_mesh::get_ao_texture()
  {
    auto t = material.streamed_texs[texture_kind::AO];
    return t ? t->query_texture() : 0;
  }

  void gl_mesh::set_streamed_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.streamed_texs[texture_kind::ALBEDO] = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_texture()
  {
    return material.streamed_texs[texture_kind::ALBEDO];
  }

  void gl_mesh::set_streamed_normal_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.streamed_texs[texture_kind::NORMAL] = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_normal_texture()
  {
    return material.streamed_texs[texture_kind::NORMAL];
  }

  void gl_mesh::set_streamed_roughness_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.streamed_texs[texture_kind::ROUGHNESS] = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_roughness_texture()
  {
    return material.streamed_texs[texture_kind::ROUGHNESS];
  }

  void gl_mesh::set_streamed_metalness_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.streamed_texs[texture_kind::METALNESS] = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_metalness_texture()
  {
    return material.streamed_texs[texture_kind::METALNESS];
  }

  void gl_mesh::set_streamed_ao_texture(std::shared_ptr<streamed_texture> tex)
  {
    material.streamed_texs[texture_kind::AO] = tex;
  }

  std::shared_ptr<streamed_texture>& gl_mesh::get_streamed_ao_texture()
  {
    return material.streamed_texs[texture_kind::AO];
  }

  void gl_mesh::query_texture_unloading()
  {
    for (const auto& t : material.streamed_texs)
      if (t)
        t->unload_if_unused();
  }

  void gl_mesh::add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh)
  {
    mesh_resource m_r(dist, lod);
    lods.push_back(m_r);

    set_vertices(mesh->get_vertices(), lod);
    set_uvs(mesh->get_uvs(), lod);
    set_normals(mesh->get_normals(), lod);
    set_tangents(mesh->get_tangents(), lod);
    set_bitangents(mesh->get_bitangents(), lod);

    std::shared_ptr<resource::gl_mesh> gl_mesh = std::static_pointer_cast<resource::gl_mesh>(mesh);
    set_vao(gl_mesh->get_vao());
  }
}
