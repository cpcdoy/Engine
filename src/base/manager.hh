#pragma once

# include <memory>
# include <vector>

namespace base
{
  template<typename T>
    class base_manager
    {
      protected:
        std::vector<std::shared_ptr<T>> managees;
        std::shared_ptr<T> current_managee;
    };
}
