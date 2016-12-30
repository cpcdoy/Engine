#pragma once

# include <vector>
# include <algorithm>
# include <functional>
# include <mutex>
# include <iostream>

# include "handler.hh"
# include "../pool/thread_pool.hh"
# include "../debug/log.hh"

namespace event
{
  class channel
  {
    public:
      template <typename T>
        class internal_channel;

      template <typename message, typename handler>
        static void add(handler* h)
        {
          internal_channel<message>::instance().add(h);
        }

      template <typename message, typename job>
        static void submit_job(job& j)
        {
          internal_channel<message>::instance().submit_job(j);
        }

      template <typename message, typename handler>
        static void remove(handler* h)
        {
          internal_channel<message>::instance().remove(h);
        }

      template <typename message>
        static void broadcast(const message& m)
        {
          internal_channel<message>::instance().broadcast(m);
        }

      template <typename message>
        class internal_channel
        {
          public:
            typedef std::function<void(const message&)> handler_callback;

            static internal_channel& instance()
            {
              static internal_channel result;
              return result;
            }

            template <typename handler>
              void add(handler* h)
              {
                std::lock_guard<std::mutex> lock(mutex);

                handlers.push_back([h](const message& m) { (*h)(m); });
                original_handlers.push_back(h);
              }

            template <typename job>
              void submit_job(job& j)
              {
                pool::thread_pool::instance().submit_job(j);
              }

            template <typename handler>
              void remove(handler* h)
              {
                std::lock_guard<std::mutex> lock(mutex);

                auto it = std::find(original_handlers.begin(), original_handlers.end(), h);
                if (it == original_handlers.end())
                  throw std::runtime_error("Tried to remove a handler that was not in the handler list");

                auto idx = it - original_handlers.begin();

                handlers.erase(handlers.begin() + idx);
                original_handlers.erase(it);
              }

            void broadcast(const message& m)
            {
              std::vector<handler_callback> localQueue(handlers.size());

              {
                debug::log::get(debug::logDEBUG) << "Event raised" << std::endl;
                std::lock_guard<std::mutex> lock(mutex);
                localQueue = handlers;
              }

              for (auto& h : localQueue)
                h(m);
            }

          private:
            std::mutex mutex;
            std::vector<handler_callback> handlers;
            std::vector<void*> original_handlers;
        };
  };
}
