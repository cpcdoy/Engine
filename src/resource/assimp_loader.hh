#pragma once

# include <vector>
# include <string>
# include <cstring>
# include <iostream>
# include <iomanip>
# include <memory>
# include <glm/glm.hpp>
# include "glm/ext.hpp"
# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif

# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <assimp/postprocess.h>

# include "model_loader.hh"
# include "mesh.hh"

namespace resource
{
  class assimp_loader : public model_loader
  {
    public:
      assimp_loader();
      virtual ~assimp_loader() override;

      void load_obj(const char* path);
      void clean_load();
      virtual bool load(const char* path) override;
      virtual std::shared_ptr<mesh> generate_mesh() override;

    private:
      void process_node(aiNode* node, const aiScene* scene);
      mesh_resource process_mesh(aiMesh* mesh, const aiScene* scene);

      std::vector<glm::vec3> out_vertices;
      std::vector<glm::vec2> out_uvs;
      std::vector<glm::vec3> out_normals;

      std::shared_ptr<mesh> m;
  };
}
