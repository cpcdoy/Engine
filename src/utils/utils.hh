#pragma once

# include <string>
# include <type_traits>
# include <vector>
# include <sstream>

namespace util
{
  std::string replace_all(std::string str, const std::string& from, const std::string& to);

  std::vector<std::string> split(const std::string& s, char delim);

  template<typename T, typename... Rest>
      struct all_same : std::false_type {};

  template<typename T, typename First>
      struct all_same<T, First> : std::is_same<T, First> {};

  template<typename T, typename First, typename... Rest>
      struct all_same<T, First, Rest...>
      : std::integral_constant<bool, std::is_same<T, First>::value && all_same<T, Rest...>::value>
      {};
}
