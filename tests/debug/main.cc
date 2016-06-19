#include "../../src/debug/debug_manager.hh"

int main(void)
{
  debug::debug_manager dm;
  dm.trigger_capture();

  return 0;
}
