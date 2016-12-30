#include "log.hh"

namespace debug
{
  std::mutex log_mutex;

  log::log()
    : out(std::cout.rdbuf())
  {
  }

  void log::set_debug(bool debug)
  {
    out.rdbuf(debug ? std::cout.rdbuf() : &nb);
  }

  std::ostream& log::get(log_level level, int indent, bool debug)
  {
    std::lock_guard<std::mutex> lock(log_mutex);

    static std::atomic<size_t> thread_counter{1};

    static thread_local size_t thread_id = 0;
    if (thread_id == 0)
      thread_id = ++thread_counter;

    static thread_local log l;

    int ind = 40;
    static thread_local int prev_ind = ind;
    static bool once = false;
    if (!debug && !once)
    {
      once = true;
      l.set_debug(debug);
      l.out << "debug : " << debug << std::endl;
      return l.out;
    }

    indent = indent < 0 ? 0 : indent;

    if (level < logINDENT)
      l.out << "[" << current_date_time() <<
        "][" << "Thread " << thread_id << "][";

    if (level == log_level::logERROR)
      l.out << COLOR_RED << "ERROR";
    else if (level == log_level::logINFO)
      l.out << COLOR_BLUE << "INFO";
    else if (level == logWARN)
      l.out << COLOR_YELLOW << "WARN";
    else if (level < logINDENT)
      l.out << COLOR_MAGENTA << "DEBUG";

    if (level < logINDENT)
      l.out << COLOR_RESET  << "] "
        << ((level == log_level::logINFO || level == log_level::logWARN) ? " " : "");
    else if (level == logREINDENT)
      l.out << COLOR_RESET << std::string(ind + prev_ind + indent, ' ');
    else
      l.out << COLOR_RESET << std::string(ind + indent, ' ');

    if (level == logINDENT)
      prev_ind = indent;

    l.messageLevel = level;
    return  l.out;
  }

  log::~log()
  {
  }
}
