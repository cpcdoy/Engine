#pragma once

# include "channel.hh"

namespace event
{
  struct engine_stop_event
  {
    bool error = false;
    std::string circumstances = "";
  };

  struct performance_statistics_event
  {
    unsigned int fps;
  };

  struct model_loading_job
  {

  };
}
