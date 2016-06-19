#include "log.hh"
#include "utils.hh"

namespace debug
{
  Log::Log()
  {
  }

  std::ostream& Log::Get(LogLevel level)
  {
    static Log l;

    std::cout << "[" << current_date_time() << 
      "][";

    if (level == LogLevel::logERROR)
      std::cout << COLOR_RED << "ERROR";
    else if (level == LogLevel::logINFO)
      std::cout << COLOR_BLUE << "INFO";
    else
      std::cout << COLOR_MAGENTA << "DEBUG";

    std::cout << COLOR_RESET  << "] ";

    l.messageLevel = level;
    return std::cout;
  }

  Log::~Log()
  {
  }
}
