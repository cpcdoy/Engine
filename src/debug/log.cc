#include "log.hh"

namespace debug
{
  log::log()
  {
  }

  std::ostream& log::get(log_level level, int indent)
  {
    static log l;

    int ind = 29;
    static int prev_ind = ind;

    indent = indent < 0 ? 0 : indent;

    if (level < logINDENT)
      std::cout << "[" << current_date_time() << 
        "][";

    if (level == log_level::logERROR)
      std::cout << COLOR_RED << "ERROR";
    else if (level == log_level::logINFO)
      std::cout << COLOR_BLUE << "INFO";
    else if (level < logINDENT)
      std::cout << COLOR_MAGENTA << "DEBUG";
    
    if (level < logINDENT)
      std::cout << COLOR_RESET  << "] "
        << (level == log_level::logINFO ? " " : "");
    else if (level == logREINDENT)
      std::cout << COLOR_RESET << std::string(ind + prev_ind + indent, ' ');
    else
      std::cout << COLOR_RESET << std::string(ind + indent, ' ');
    
    if (level == logINDENT)
      prev_ind = indent; // + len of the string to be displayed;

    l.messageLevel = level;
    return std::cout;
  }

  log::~log()
  {
  }
}
