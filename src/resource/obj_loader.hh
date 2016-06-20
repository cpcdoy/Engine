#pragma once

# include <vector>
# include <string>
# include <cstring>
# include <iostream>
# include <glm/glm.hpp>
# include "loader.hh"

namespace resource
{
  class obj_loader : public loader
  {
    public:
      obj_loader();
      ~obj_loader();
      
      void load_obj(const char* path);
      virtual void load(const char* path) override;

    private:
      std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
      std::vector<glm::vec3> temp_vertices;
      std::vector<glm::vec2> temp_uvs;
      std::vector<glm::vec3> temp_normals;
  };
}
