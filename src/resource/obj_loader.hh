#pragma once

# include <vector>
# include <string>
# include <cstring>
# include <iostream>
# include <iomanip>
# include <memory>
# include <glm/glm.hpp>

# include "loader.hh"
# include "mesh.hh"

namespace resource
{
  class obj_loader : public loader
  {
    public:
      obj_loader();
      virtual ~obj_loader() override;

      void load_obj(const char* path);
      void clean_load();
      virtual bool load(const char* path) override;
      virtual std::shared_ptr<mesh> generate_mesh() override;

    private:
      std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
      std::vector<glm::vec3> temp_vertices;
      std::vector<glm::vec2> temp_uvs;
      std::vector<glm::vec3> temp_normals;

      std::vector<glm::vec3> out_vertices;
      std::vector<glm::vec2> out_uvs;
      std::vector<glm::vec3> out_normals;
  };
}
