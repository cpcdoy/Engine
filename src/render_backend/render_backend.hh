#pragma once

# include <string>

# include "../debug/log.hh"

namespace render_backend
{
  class render_backend
  {
    public:
      render_backend()
      {
      }

      ~render_backend()
      {
      }

      virtual bool init_backend()
      {
        return false;
      }

      virtual std::string get_backend_id()
      {
        return backend_id;
      }

    protected:
      std::string backend_id = "virtual empty render backend";
  };
}
