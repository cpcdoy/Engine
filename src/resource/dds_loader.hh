#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif
# include <vector>
# include <algorithm>
# include <fstream>
# include <cstring>

# include "../debug/log.hh"
# include "texture_loader.hh"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

namespace resource
{
  class dds_loader : public texture_loader
  {
    public:
      dds_loader();
      virtual ~dds_loader() override;

      virtual bool load(const char* path) override;
      virtual unsigned char* get_generated_texture() override;
      virtual void clean() override;
    private:
      std::vector<unsigned char*> texs;
  };
}
