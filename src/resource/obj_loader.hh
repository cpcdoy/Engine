#pragma once

# include <vector>
# include <string>
# include <cstring>
# include <iostream>
# include <iomanip>
# include <memory>
# include <glm/glm.hpp>
# include "glm/ext.hpp"

# include "model_loader.hh"
# include "mesh.hh"

namespace resource
{
  class obj_loader : public model_loader
  {
    public:
      obj_loader();
      virtual ~obj_loader() override;

      void load_obj(const char* path);
      void clean_load();
      virtual bool load(const char* path) override;
      virtual std::shared_ptr<mesh> generate_mesh() override;

    private:
      void compute_tangent_basis();
      template<typename T>
        bool is_near(T v0, T v1);
      bool is_similar_to_any_vertex_element(glm::vec3 v, glm::vec2 uv, glm::vec3 n, size_t& index);
      void average_tangents();

      std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
      std::vector<glm::vec3> tmp_vertices;
      std::vector<glm::vec2> tmp_uvs;
      std::vector<glm::vec3> tmp_normals;

      std::vector<glm::vec3> out_vertices;
      std::vector<glm::vec2> out_uvs;
      std::vector<glm::vec3> out_normals;

      std::vector<glm::vec3> out_tangents;
      std::vector<glm::vec3> out_bitangents;

      std::vector<glm::vec3> tmp_tangents;
      std::vector<glm::vec3> tmp_bitangents;

      glm::vec3 center;
      glm::vec3 max_vertex;

      struct aabb aabb;
  };
}
