#pragma once

# include <functional>

# include "../utils/concurrent_queue.hh"

namespace pool
{
  class worker
  {
    public:
      void process()
      {
        while (true)
        {
          std::unique_lock<std::mutex> lock(queue_mutex);
          cond_var.wait(lock, [this] { return !empty(); });

          pop()();
        }
      }

    private:
      std::condition_variable cond_var;
      std::mutex queue_mutex;
  };
}
