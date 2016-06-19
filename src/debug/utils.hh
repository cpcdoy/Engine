#include <string>

#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RED "\x1b[31m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

namespace debug
{
  const std::string current_date_time();
}
