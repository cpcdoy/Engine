#include "debug_manager.hh"
#include "log.hh"

namespace debug
{
  debug_manager::debug_manager(std::string path)
  {
    log::get(log_level::logINFO) << "Loading the debug manager"
                                 << std::endl;
    handle = dlopen(path.c_str(), RTLD_LAZY);

    if (!handle)
      log::get(log_level::logERROR) << "Cannot find/open the debugger's shared object"
                                           << std::endl;
    else
    {
      RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(handle, "RENDERDOC_GetAPI");

      if (!RENDERDOC_GetAPI)
      {
        log::get(log_level::logERROR) << "Cannot find symbol RENDERDOC_GetAPI in " << path
          << std::endl;
      }
      else
      {
        assert(RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_0_0, (void **)&rdoc_api) == 1);

        assert(rdoc_api->StartFrameCapture != NULL && rdoc_api->EndFrameCapture != NULL);

        int major = 999;
        int minor = 999;
        int patch = 999;

        rdoc_api->GetAPIVersion(&major, &minor, &patch);

        assert(major == 1 && minor >= 0 && patch >= 0);

        rdoc_api->SetLogFilePathTemplate("capture_logs");

        rdoc_api->SetFocusToggleKeys(NULL, 0);

        RENDERDOC_InputButton captureKey = eRENDERDOC_Key_F11;
        rdoc_api->SetCaptureKeys(&captureKey, 1);

        rdoc_api->SetCaptureOptionU32(eRENDERDOC_Option_AllowVSync, 1);
        rdoc_api->SetCaptureOptionU32(eRENDERDOC_Option_SaveAllInitials, 1);

        rdoc_api->MaskOverlayBits(eRENDERDOC_Overlay_None, eRENDERDOC_Overlay_None);
      }
    }
  }

  void debug_manager::trigger_capture()
  {
    if (handle)
    {
      rdoc_api->TriggerCapture();
      log::get(log_level::logDEBUG) << "Triggered frame capture" << std::endl;
      log::get(log_level::logINDENT) << "To be saved in ./"
                                     << rdoc_api->GetLogFilePathTemplate()
                                     << std::endl;
    }
    else
      log::get(log_level::logERROR) << "Error: cannot trigger a capture because the debug API isn't loaded"
                                           << std::endl;
  }

  debug_manager::~debug_manager()
  {
    if (handle)
    {
      rdoc_api->Shutdown();
      dlclose(handle);
      log::get(log_level::logDEBUG) << "Shutting down the debug API"
        << std::endl;
    }
  }
}
