#pragma once

//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

# include <queue>
# include <thread>
# include <mutex>
# include <condition_variable>

namespace util
{
  template <typename T>
    class streaming_queue
    {
      public:
        T pop() 
        {
          std::unique_lock<std::mutex> mlock(mutex_);
          while (queue.empty())
          {
            cond_.wait(mlock);
          }
          auto val = queue.front();
          queue.pop();
          return val;
        }

        void pop(T& item)
        {
          std::unique_lock<std::mutex> mlock(mutex_);
          while (queue.empty())
          {
            cond_.wait(mlock);
          }
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
    };
}
