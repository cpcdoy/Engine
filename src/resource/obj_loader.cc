#include "obj_loader.hh"

namespace resource
{
  obj_loader::obj_loader()
    : model_loader()
  {
    loader_id = "obj_loader";
  }

  obj_loader::~obj_loader()
  {
    clean_load();

    out_vertices.clear();
    out_uvs.clear();
    out_normals.clear();
    out_tangents.clear();
    out_bitangents.clear();
  }

  void obj_loader::load_obj(const char *path)
  {
    load(path);
  }

  inline
    void obj_loader::clean_load()
    {
      tmp_vertices.clear();
      tmp_uvs.clear();
      tmp_normals.clear();

      vertex_indices.clear();
      uv_indices.clear();
      normal_indices.clear();
    }

  std::shared_ptr<mesh> obj_loader::generate_mesh()
  {
    debug::log::get(debug::logINDENT, 5) << "Generating mesh" << std::endl;

    std::shared_ptr<mesh> m = std::make_shared<mesh>();

    m->set_vertices(out_vertices);
    m->set_uvs(out_uvs);
    m->set_normals(out_normals);
    m->set_tangents(out_tangents);
    m->set_bitangents(out_bitangents);
    m->set_aabb(max_vertex - center, center);

    return m;
  }

  bool obj_loader::load(const char* path)
  {
    glm::vec3 min_vertex;
    debug::log::get(debug::logINFO) << "Loading OBJ file " << path << std::endl;

    out_vertices.clear();
    out_uvs.clear();
    out_normals.clear();
    out_tangents.clear();

    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
      debug::log::get(debug::logERROR) << "Cannot open the file (most likely not found): " << path << std::endl;
      return false;
    }

    while (1)
    {
      char lineHeader[128];
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF)
        break;

      if (strcmp(lineHeader, "v") == 0)
      {
        glm::vec3 vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        tmp_vertices.push_back(vertex);
        if (vertex.x > max_vertex.x)
          max_vertex.x = vertex.x;
        if (vertex.y > max_vertex.y)
          max_vertex.y = vertex.y;
        if (vertex.z > max_vertex.z)
          max_vertex.z = vertex.z;

        if (vertex.x < min_vertex.x)
          min_vertex.x = vertex.x;
        if (vertex.y < min_vertex.y)
          min_vertex.y = vertex.y;
        if (vertex.z < min_vertex.z)
          min_vertex.z = vertex.z;
      }
      else if (strcmp(lineHeader, "vt") == 0)
      {
        glm::vec2 uv;
        fscanf(file, "%f %f\n", &uv.x, &uv.y);
        uv.y = -uv.y;
        tmp_uvs.push_back(uv);
      }
      else if (strcmp(lineHeader, "vn") == 0)
      {
        glm::vec3 normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
        tmp_normals.push_back(normal);
      }
      else if (strcmp(lineHeader, "f") == 0)
      {
        std::string vertex1, vertex2, vertex3;
        unsigned int vertex_index[3], uv_index[3], normal_index[3];
        int curr_pos = ftell(file);
        int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertex_index[0], &uv_index[0], &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1], &vertex_index[2], &uv_index[2], &normal_index[2]);

        if (matches != 9)
        {
          uv_index[0] = 0;
          uv_index[1] = 0;
          uv_index[2] = 0;
          fseek(file, curr_pos, SEEK_SET);
          matches = fscanf(file, "%u//%u %u//%u %u//%u\n", &vertex_index[0], &normal_index[0], &vertex_index[1], &normal_index[1], &vertex_index[2], &normal_index[2]);
          if (matches != 6)
            debug::log::get(debug::logERROR) << "File can't be processed, try triangularizing the faces"
              << std::endl;
          return false;
        }
        vertex_indices.push_back(vertex_index[0]);
        vertex_indices.push_back(vertex_index[1]);
        vertex_indices.push_back(vertex_index[2]);
        uv_indices.push_back(uv_index[0]);
        uv_indices.push_back(uv_index[1]);
        uv_indices.push_back(uv_index[2]);
        normal_indices.push_back(normal_index[0]);
        normal_indices.push_back(normal_index[1]);
        normal_indices.push_back(normal_index[2]);
      }
      else
      {
        char b[1000];
        fgets(b, 1000, file);
      }
    }
    center = (max_vertex + min_vertex) / 2.0f;
    //std::cout << "center v " << center.x << " " << center.y << " " << center.z << std::endl;

    for (unsigned int i = 0; i < vertex_indices.size(); i++)
    {
      unsigned int vertex_index = vertex_indices[i];
      unsigned int uv_index = uv_indices[i];
      unsigned int normal_index = normal_indices[i];

      glm::vec3 vertex = tmp_vertices[vertex_index - 1];
      glm::vec2 uv = tmp_uvs[uv_index - 1];
      glm::vec3 normal = tmp_normals[normal_index - 1];

      out_vertices.push_back(vertex);
      out_uvs.push_back(uv);
      out_normals.push_back(normal);
    }

    clean_load();

    fclose(file);

    debug::log::get(debug::logINFO) << "Loaded OBJ file " << path << "" << std::endl;
    debug::log::get(debug::logINDENT, 5) << "Stats:" << std::endl;
    debug::log::get(debug::logINDENT, 6 + 5) << out_vertices.size() << " vertices" << std::endl;
    debug::log::get(debug::logREINDENT) << out_vertices.size() / 3 << " triangles" << std::endl;

    //compute_tangent_basis();
    //average_tangents();

    return true;
  }

  void obj_loader::compute_tangent_basis()
  {
    debug::log::get(debug::logINDENT, 5) << "Generating tangents and bitangents" << std::endl;

    out_tangents.clear();
    out_bitangents.clear();

    for (size_t i = 0; i < out_vertices.size(); i += 3)
    {
      const auto& v0 = out_vertices[i];
      const auto& v1 = out_vertices[i + 1];
      const auto& v2 = out_vertices[i + 2];

      const auto& uv0 = out_uvs[i];
      const auto& uv1 = out_uvs[i + 1];
      const auto& uv2 = out_uvs[i + 2];

      const auto& dd_v0 = v1 - v0;
      const auto& dd_v1 = v2 - v0;

      const auto& dd_uv0 = uv1 - uv0;
      const auto& dd_uv1 = uv2 - uv0;

      float r = 1.0f / (dd_uv0.x * dd_uv1.y - dd_uv0.y * dd_uv1.x);
      glm::vec3 tangent = glm::normalize((dd_v0 * dd_uv1.y - dd_v1 * dd_uv0.y) * r);
      glm::vec3 bitangent = glm::normalize((dd_v1 * dd_uv0.x - dd_v0 * dd_uv1.x) * r);

      tmp_tangents.push_back(tangent);
      tmp_tangents.push_back(tangent);
      tmp_tangents.push_back(tangent);

      tmp_bitangents.push_back(bitangent);
      tmp_bitangents.push_back(bitangent);
      tmp_bitangents.push_back(bitangent);
    }

    debug::log::get(debug::logINDENT, 5) << "tang " << out_tangents.size() << std::endl;
    debug::log::get(debug::logINDENT, 5) << "bitang " << out_bitangents.size() << std::endl;
  }
}
