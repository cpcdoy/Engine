#pragma once

# include <string>

namespace ui
{
  class base_ui
  {
    public:
      base_ui()
      {
      }

      ~base_ui()
      {
      }

      virtual bool init_base_ui()
      {
        return false;
      }

      virtual bool create_window(int height, int width, std::string title)
      {
        (void)height;
        (void)width;
        (void)title;
        return false;
      }

      virtual std::string get_base_ui_id()
      {
        return base_ui_id;
      }

      virtual bool satisfies_running_condition()
      {
        return false;
      }

      virtual void update_ui()
      {
      }

      virtual void set_backend_context_version(int major, int minor)
      {
        major_backend_version = major;
        minor_backend_version = minor;
      }

    protected:
      std::string base_ui_id = "virtual empty base ui";
      int major_backend_version, minor_backend_version;
  };
}
