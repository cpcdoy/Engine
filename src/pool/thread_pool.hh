#pragma once

# include <future>
# include <chrono>
# include <vector>
# include <functional>

# include "../utils/concurrent_queue.hh"
# include "../debug/log.hh"

namespace pool
{
  class thread_pool : public util::concurrent_queue<std::function<void()>>
  {
    public:
      thread_pool(unsigned int thread_nb = 0)
      {
        if (thread_nb)
          this->thread_nb = thread_nb;
        else
          this->thread_nb = std::thread::hardware_concurrency() - 1;
        worker_queue.reserve(this->thread_nb);

        debug::log::get(debug::logDEBUG) << this->thread_nb << " available worker threads" << std::endl;
        for (unsigned int i = 0; i < this->thread_nb; i++)
          launch_worker();
      }

      static thread_pool& instance()
      {
        static thread_pool pool;
        return pool;
      }

      void submit_job(std::function<void()>& j)
      {
        push(j);

        //if (!worker_queue.size())
          //launch_worker();
      }

      void launch_worker()
      {
        worker_queue.push_back(std::async(std::launch::async, &thread_pool::process, this));
      }

      void process()
      {
        debug::log::get(debug::logDEBUG) << "Launching a worker thread..." << std::endl;

        while (!stop)
          pop_stoppable()();

        debug::log::get(debug::logDEBUG) << "Stopping worker thread..." << std::endl;
      }

      ~thread_pool()
      {
        stop = true;
        for (auto& f : worker_queue)
          push([] { });
      }

    private:
      unsigned int thread_nb;

      std::vector<std::future<void>> worker_queue;
  };
}
