#pragma once

# include <vector>
# include <glm/glm.hpp>
# include <glm/gtx/transform.hpp>

# include "entity.hh"

namespace resource
{
  class mesh;

  enum mesh_mode
  {
    static_mode,
    dynamic_mode,
    all,
  };

  struct mesh_resource
  {
    mesh_resource();
    mesh_resource(int dist, int level)
    {
      this->dist = dist;
      this->level = level;
    }

    int dist;
    int level = 0;

    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> vertices;

    std::vector<mesh> sub_meshes;
  };

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

      void compute_current_lod(glm::vec3 cam_pos);
      int get_current_log();

      glm::mat4 get_model();

    protected:
      int lod;

      std::vector<mesh_resource> lods;

      std::string mesh_backend_id = "gl";

      glm::mat4 model;
  };
}
