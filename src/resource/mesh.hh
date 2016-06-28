#pragma once

# include <vector>
# include <glm/glm.hpp>
# include <glm/gtx/transform.hpp>


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

      std::vector<glm::vec3> get_vertices();
      std::vector<glm::vec2> get_uvs();
      std::vector<glm::vec3> get_normals();

      void set_pos(glm::vec3 pos);
      void set_scale(glm::vec3 scale);

      glm::mat4 get_model();

    protected:
      int lod;

      std::vector<glm::vec2> uvs;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec3> vertices;

      std::string mesh_backend_id = "gl";
      
      glm::mat4 model;
  };
}
