#include "dds_loader.hh"

namespace resource
{
  dds_loader::dds_loader()
    : texture_loader()
  {
    loader_id = "dds_loader";
  }

  dds_loader::~dds_loader()
  {
    debug::log::get(debug::log_level::logINFO) << "Cleaning the " << loader_id << "'s resources" << std::endl;
    std::for_each(texs.begin(), texs.end(), [](unsigned char* tex)
        {
          delete [] tex;
        });
  }

  bool dds_loader::load(const char* path)
  {
    std::ifstream f(path, std::ios::binary);

    char magic[4];
    f.read(magic, 4);

    if (std::strncmp(magic, "DDS ", 4))
    {
      debug::log::get(debug::log_level::logERROR) << path << " is corrupted or the format is not correct" << std::endl;
      return false;
    }

    char header[124];
    f.read(header, 124);

    height = *reinterpret_cast<unsigned int*>(&header[8]);
    width = *reinterpret_cast<unsigned int*>(&header[12]);
    int linear_size = *reinterpret_cast<unsigned int*>(&header[16]);
    mipmap_levels = *reinterpret_cast<unsigned int*>(&header[24]);
    int four_CC = *reinterpret_cast<unsigned int*>(&header[80]);

    unsigned int buf_size = mipmap_levels > 1 ? linear_size * 2 : linear_size;

    tex = new unsigned char[buf_size];
    f.read(reinterpret_cast<char*>(tex), buf_size);

    f.close();

    if (four_CC == FOURCC_DXT1)
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    else if (four_CC == FOURCC_DXT3)
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    else if (four_CC == FOURCC_DXT5)
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    else
    {
      debug::log::get(debug::log_level::logERROR) << "Unsupported DDS compression format : try using DXT1, DXT3 or DXT5" << std::endl;
      delete [] tex;
      return false;
    }

		block_size = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    compressed = true;

    return tex != nullptr;
  }

  unsigned char* dds_loader::get_generated_texture()
  {
    return tex;
  }

  void dds_loader::clean()
  {
    delete [] tex;
  }
}
