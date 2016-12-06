#include "assimp_loader.hh"

namespace resource
{
  assimp_loader::assimp_loader()
    : model_loader()
  {
    loader_id = "assimp loader";
  }

  assimp_loader::~assimp_loader()
  {
    out_vertices.clear();
    out_uvs.clear();
    out_normals.clear();
  }

  void assimp_loader::load_obj(const char *path)
  {
    load(path);
  }

  bool assimp_loader::load(const char *p)
  {
    m = std::make_shared<mesh>();
    Assimp::Importer importer;
    std::string path = p;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_ImproveCacheLocality | aiProcess_FixInfacingNormals | aiProcess_FindDegenerates | aiProcess_FindInvalidData | aiProcess_GenUVCoords | aiProcess_TransformUVCoords);

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      debug::log::get(debug::logERROR) << "Assimp failed to load the model at \"" << path << "\" :" << std::endl;
      debug::log::get(debug::logINDENT, 5) << importer.GetErrorString() << std::endl;

      return false;
    }

    process_node(scene->mRootNode, scene);

    const auto& submeshes = m->get_submeshes();
    debug::log::get(debug::logINFO) << "Loaded OBJ file " << path << "" << std::endl;
    debug::log::get(debug::logINDENT, 5) << "Stats:" << std::endl;
    debug::log::get(debug::logINDENT, 6 + 5) << submeshes.size() << " mesh" << (submeshes.size() != 1 ? "es:" : ":")  << std::endl;
    size_t i = 0;
    for (const auto& subm : submeshes)
      debug::log::get(debug::logINDENT, 6 + 5 + 5) << "Submesh " << i++ << ": " << subm.vertices.size() << " vertices" << std::endl;

    return true;
  }

  void assimp_loader::process_node(aiNode* node, const aiScene* scene)
  {
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      m->add_submesh(process_mesh(mesh, scene));
    }

    for(GLuint i = 0; i < node->mNumChildren; i++)
      process_node(node->mChildren[i], scene);
  }

  mesh_resource assimp_loader::process_mesh(aiMesh* mesh, const aiScene* scene)
  {
    mesh_resource sub;
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
      glm::vec3 vector;
      // Positions
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      sub.vertices.push_back(vector);
      // Normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      sub.normals.push_back(vector);
      // UVs
      glm::vec2 uv;
      if(mesh->mTextureCoords[0])
      {
        uv.x = mesh->mTextureCoords[0][i].x;
        uv.y = mesh->mTextureCoords[0][i].y;
      }
      else
        uv = glm::vec2(0.0f, 0.0f);
      sub.uvs.push_back(uv);
    }

    return sub;
  }

  std::shared_ptr<mesh> assimp_loader::generate_mesh()
  {
    debug::log::get(debug::logINDENT, 5) << "Generating mesh" << std::endl;

    return m;
  }
}
