#include <string>
#include <sstream>
#include <iostream>

namespace debug
{
  enum LogLevel
  {
    logERROR,
    logWARNING,
    logINFO,
    logDEBUG,
    logDEBUG1,
    logDEBUG2,
    logDEBUG3,
    logDEBUG4
  };

  class Log
  {
    public:
      Log();
      ~Log();
      static std::ostream& Get(LogLevel level = logINFO);

    private:
      LogLevel messageLevel;
  };
}
