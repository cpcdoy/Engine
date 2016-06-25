#pragma once

# include <vector>
# include <glm/glm.hpp>

# include "entity.hh"

namespace resource
{
  class mesh : public entity
  {
    public:
      mesh();
      ~mesh();

      void set_vertices(std::vector<glm::vec3> v);
      void set_uvs(std::vector<glm::vec2> u);
      void set_normals(std::vector<glm::vec3> n);

    protected:
      int lod;

      std::vector<glm::vec2> uvs;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec3> vertices;

      std::string mesh_backend_id = "gl";
  };
}
