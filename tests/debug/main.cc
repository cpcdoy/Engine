#include "../../src/engine/engine.hh"
#include <functional>

int main(void)
{
  engine::engine e;

  //Optional
  e.set_engine_render_context(render_backend::render_plugins::OpenGL, 3, 3);
  //Optional
  e.set_window_properties(800, 600, "test");

  e.init();

  e.load_mesh("tests/res/test.obj");
  e.load_mesh("tests/res/test.obj");
  e.load_mesh("lol.obj");

  auto f = std::function<void()>([](){ int a = 1 + 2; (void)a; });

  e.run(f);
  return 0;
}
