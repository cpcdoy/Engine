#pragma once

# include <queue>
# include <thread>
# include <mutex>
# include <condition_variable>
# include <memory>

namespace util
{
  template <typename T>
    class concurrent_queue
    {
      public:
        T pop()
        {
          std::unique_lock<std::mutex> mlock(mutex_);
          cond_.wait(mlock, [this] { return !queue.empty(); });
          auto val = queue.front();
          queue.pop();
          return val;
        }

        T pop_stoppable()
        {
          std::unique_lock<std::mutex> mlock(mutex_);
          cond_.wait(mlock, [this] { return !queue.empty() || stop; });
          auto val = queue.front();
          queue.pop();
          return val;
        }

        void pop(T& item)
        {
          std::unique_lock<std::mutex> mlock(mutex_);
          while (queue.empty())
            cond_.wait(mlock);

          item = queue.front();
          queue.pop();
        }

        void push(const T& item)
        {
          std::unique_lock<std::mutex> mlock(mutex_);
          queue.push(item);
          mlock.unlock();
          cond_.notify_one();
        }

        bool empty()
        {
          return queue.empty();
        }

      protected:
        std::queue<T> queue;
        std::mutex mutex_;
        std::condition_variable cond_;

        bool stop = false;
    };
}
