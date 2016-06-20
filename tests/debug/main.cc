#include "../../src/debug/debug_manager.hh"
#include "../../src/resource/obj_loader.hh"

int main(void)
{
  debug::debug_manager dm;
  dm.trigger_capture();

  resource::obj_loader obj_loader;
  obj_loader.load_obj("tests/res/test.obj");

  return 0;
}
