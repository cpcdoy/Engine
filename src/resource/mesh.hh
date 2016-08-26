#pragma once

# include <vector>
# include <memory>

# include <glm/glm.hpp>
# include <glm/gtx/transform.hpp>

# include "../debug/log.hh"
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

      void set_vertices(std::vector<glm::vec3> v, int lod);
      void set_uvs(std::vector<glm::vec2> u, int lod);
      void set_normals(std::vector<glm::vec3> n, int lod);

      std::vector<glm::vec3> get_vertices();
      std::vector<glm::vec2> get_uvs();
      std::vector<glm::vec3> get_normals();

      glm::vec3 get_pos();
      void set_pos(glm::vec3 pos);
      void set_scale(glm::vec3 scale);

      void compute_current_lod(glm::vec3 cam_pos);
      int get_current_lod();

      virtual void add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh);

      glm::mat4 get_model();

      void set_lod_distance(int lod, int dist);

    protected:
      int lod;

      std::vector<mesh_resource> lods;

      std::string mesh_backend_id = "gl";

      glm::mat4 model;
      glm::vec3 pos;
  };
}
