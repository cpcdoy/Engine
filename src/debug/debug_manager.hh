#pragma once

# include <iostream>
# include <cassert>
# include <dlfcn.h>
# include "renderdoc_app.h"
# include "../event/events.hh"
# include "log.hh"

namespace debug
{
  class debug_manager
  {
    public:
      ~debug_manager();
      debug_manager(std::string path = std::string("libs/librenderdoc.so"));

      void trigger_capture();

      void operator()(const event::engine_stop_event&)
      {
        debug::log::get(debug::logINFO) << "Debug manager shutting down..." << std::endl;
      }

    private:
      void* handle;
      RENDERDOC_API_1_0_0 *rdoc_api = nullptr;
      pRENDERDOC_GetAPI RENDERDOC_GetAPI;
  };
}
