#include "log.hh"

namespace debug
{
  log::log()
  {
  }

  std::ostream& log::get(log_level level, int indent)
  {
    static std::atomic<size_t> thread_counter{0};

    static thread_local size_t thread_id = 0;
    if (thread_id == 0)
      thread_id = thread_counter++;

    static thread_local log l;

    int ind = 29;
    static thread_local int prev_ind = ind;

    indent = indent < 0 ? 0 : indent;

    if (level < logINDENT)
      std::cout << "[" << current_date_time() << 
        "][" << "Thread " << thread_id << "][";

    if (level == log_level::logERROR)
      std::cout << COLOR_RED << "ERROR";
    else if (level == log_level::logINFO)
      std::cout << COLOR_BLUE << "INFO";
    else if (level == logWARN)
      std::cout << COLOR_YELLOW << "WARN";
    else if (level < logINDENT)
      std::cout << COLOR_MAGENTA << "DEBUG";
    
    if (level < logINDENT)
      std::cout << COLOR_RESET  << "] "
        << ((level == log_level::logINFO || level == log_level::logWARN) ? " " : "");
    else if (level == logREINDENT)
      std::cout << COLOR_RESET << std::string(ind + prev_ind + indent, ' ');
    else
      std::cout << COLOR_RESET << std::string(ind + indent, ' ');
    
    if (level == logINDENT)
      prev_ind = indent;

    l.messageLevel = level;
    return std::cout;
  }

  log::~log()
  {
  }
}
