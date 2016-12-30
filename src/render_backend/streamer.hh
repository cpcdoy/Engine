#pragma once

# include <vector>
# include <string>
# include <algorithm>

# include "../utils/concurrent_queue.hh"

namespace render_backend
{
  template <typename T>
    class streamer : public util::concurrent_queue<T>
  {
    public:
      virtual void add_streamed_element(std::string path)
      {
        elements.push_back(path);
      }

      virtual void remove_streamed_element(std::string path)
      {
        auto elem = std::find(elements.begin(), elements.end(), path);
        if (elem != elements.end())
          elements.erase(elem);
      }

      virtual void stream()
      {
      }

    protected:
      std::vector<std::string> elements;
  };
}
