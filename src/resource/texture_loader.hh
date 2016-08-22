#pragma once

# include <memory>

namespace resource
{
  class texture_loader
  {
    public:
      texture_loader()
      {
      }

      virtual ~texture_loader()
      {
      }

      virtual bool load(const char* path)
      {
        (void)path;
        return false;
      }

      virtual unsigned char* get_generated_texture()
      {
        return nullptr;
      }

      virtual void clean()
      {
      }

      virtual int get_height()
      {
        return height;
      }

      virtual int get_width()
      {
        return width;
      }

      virtual std::string get_loader_id()
      {
        return loader_id;
      }

      virtual int get_mipmap_levels()
      {
        return mipmap_levels;
      }

      virtual bool is_compressed()
      {
        return compressed;
      }

      virtual int get_block_size()
      {
        return block_size;
      }

      virtual int get_format()
      {
        return format;
      }

    protected:
      std::string loader_id = "virtual empty texture loader";

      unsigned char* tex;
      int width, height;
      int channels;
      int mipmap_levels;
      int block_size;
      int format;
      bool compressed;
  };
}
