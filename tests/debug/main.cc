#include "../../src/debug/debug_manager.hh"
#include "../../src/resource/resource_manager.hh"
#include "../../src/ui/ui_manager.hh"

int main(void)
{
  /*resource::obj_loader obj_loader;
  obj_loader.load_obj("tests/res/test.obj");
  obj_loader.generate_mesh();

  obj_loader.load_obj("lol.obj");*/

  resource::resource_manager rm;

  rm.load("tests/res/test.obj");
  rm.load("lol.obj");

  ui::ui_manager ui_manager;

  ui_manager.init_ui();
  ui_manager.create_window(200, 200, "test");

  while (ui_manager.ui_satisfies_running_condition())
    ui_manager.update_ui();

  return 0;
}
