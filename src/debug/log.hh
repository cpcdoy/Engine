#pragma once

# include <string>
# include <sstream>
# include <iostream>
# include <iomanip>
# include <atomic>

# include "utils.hh"


namespace debug
{
  class null_buffer : public std::streambuf
  {
    public:
      int overflow(int c)
      {
        return c;
      }
  };

  enum log_level
  {
    logERROR,
    logINFO,
    logDEBUG,
    logWARN,
    logINDENT,
    logREINDENT,
  };

  class log
  {
    public:
      log();
      log(bool);
      ~log();
      log& operator<<(std::string& s);
      static std::ostream& get(log_level level = logINFO, int indent = 0, bool debug = true);
      void set_debug(bool);

    private:
      log_level messageLevel;

      null_buffer nb;
      std::streambuf* buf;
      std::ostream out;
  };
}
