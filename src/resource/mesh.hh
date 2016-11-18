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

  struct aabb
  {
    aabb()
    {
    }
    aabb(glm::vec3 center, glm::vec3 half_extent)
      : center(center),
      half_extent(half_extent)
    {
    }

    glm::vec3 center;

    glm::vec3 half_extent;
  };

  struct mesh_resource
  {
    mesh_resource();
    mesh_resource(int dist, int level)
      : dist(dist),
      level(level)
    {
    }

    int dist;
    int level = 0;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    std::vector<mesh> sub_meshes;

    struct aabb aabb;
  };

  class mesh : public entity
  {
    public:
      mesh();
      ~mesh();

      void set_vertices(std::vector<glm::vec3> v, int lod = 0);
      void set_uvs(std::vector<glm::vec2> u, int lod = 0);
      void set_normals(std::vector<glm::vec3> n, int lod = 0);
      void set_tangents(std::vector<glm::vec3> t, int lod = 0);
      void set_bitangents(std::vector<glm::vec3> bt, int lod = 0);

      std::vector<glm::vec3> get_vertices();
      std::vector<glm::vec2> get_uvs();
      std::vector<glm::vec3> get_normals();
      std::vector<glm::vec3> get_tangents();
      std::vector<glm::vec3> get_bitangents();

      glm::vec3 get_pos();
      void set_pos(glm::vec3 pos);
      void set_scale(glm::vec3 scale);
      void set_rotation(float ang, glm::vec3 axis);

      void set_aabb(glm::vec3 center, glm::vec3 ext);
      struct aabb get_aabb();

      void compute_current_lod(glm::vec3 cam_pos);
      int get_current_lod();

      virtual void add_lod(int dist, int lod, std::shared_ptr<mesh>& mesh);
      virtual void query_texture_unloading()
      {
      }

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
