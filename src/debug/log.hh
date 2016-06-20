#pragma once

# include <string>
# include <sstream>
# include <iostream>
# include <iomanip>

# include "utils.hh"


namespace debug
{
  enum log_level
  {
    logERROR,
    logWARNING,
    logINFO,
    logDEBUG,
    logDEBUG1,
    logDEBUG2,
    logDEBUG3,
    logDEBUG4,
    logINDENT,
    logREINDENT,
  };

  class log
  {
    public:
      log();
      ~log();
      log& operator<<(std::string& s);
      static std::ostream& get(log_level level = logINFO, int indent = 0);

    private:
      log_level messageLevel;
  };
}
