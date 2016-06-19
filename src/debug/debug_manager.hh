#include <iostream>
#include <cassert>
#include <dlfcn.h>
#include "renderdoc_app.h"

namespace debug
{
  class debug_manager
  {
    public:
      ~debug_manager();
      debug_manager(std::string path = std::string("libs/librenderdoc.so"));

      void trigger_capture();
    private:
      void* handle;
      RENDERDOC_API_1_0_0 *rdoc_api = nullptr;
      pRENDERDOC_GetAPI RENDERDOC_GetAPI;
  };
}
