#include "obj_loader.hh"
#include <iomanip>

namespace resource
{
  obj_loader::obj_loader()
    : loader()
  {}

  obj_loader::~obj_loader()
  {}

  void obj_loader::load_obj(const char *path)
  {
    load(path);
  }

  void obj_loader::load(const char* path)
  {
    debug::log::get(debug::logINFO) << "Loading OBJ file " << path << std::endl;

    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
      debug::log::get(debug::logERROR) << "Cannot open the file : " << path << std::endl;
      return;
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
        temp_vertices.push_back(vertex);
      }
      else if (strcmp(lineHeader, "vt") == 0)
      {
        glm::vec2 uv;
        fscanf(file, "%f %f\n", &uv.x, &uv.y);
        uv.y = -uv.y;
        temp_uvs.push_back(uv);
      }
      else if (strcmp(lineHeader, "vn") == 0)
      {
        glm::vec3 normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
        temp_normals.push_back(normal);
      }
      else if (strcmp(lineHeader, "f") == 0)
      {
        std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
        int curr_pos = ftell(file);
        int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

        if (matches != 9)
        {
          uvIndex[0] = 0;
          uvIndex[1] = 0;
          uvIndex[2] = 0;
          fseek(file, curr_pos, SEEK_SET);
          matches = fscanf(file, "%u//%u %u//%u %u//%u\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
          if (matches != 6)
            debug::log::get(debug::logERROR) << "File can't be read, try triangularizing the faces"
                                                   << std::endl;
          return;
        }
        vertex_indices.push_back(vertexIndex[0]);
        vertex_indices.push_back(vertexIndex[1]);
        vertex_indices.push_back(vertexIndex[2]);
        uv_indices.push_back(uvIndex[0]);
        uv_indices.push_back(uvIndex[1]);
        uv_indices.push_back(uvIndex[2]);
        normal_indices.push_back(normalIndex[0]);
        normal_indices.push_back(normalIndex[1]);
        normal_indices.push_back(normalIndex[2]);
      }
      else
      {
        char b[1000];
        fgets(b, 1000, file);
      }
    }

    debug::log::get(debug::logINFO) << "Loaded OBJ file " << path << "" << std::endl;
    debug::log::get(debug::logINDENT) << "Stats:" << std::endl;
    debug::log::get(debug::logINDENT, 6) << vertex_indices.size() << " vertices" << std::endl;
    debug::log::get(debug::logREINDENT) << vertex_indices.size() / 3 << " tris" << std::endl;
  }
}
