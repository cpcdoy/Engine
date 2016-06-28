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

  mesh m = e.load_mesh("tests/res/test.obj");
  m->set_pos(glm::vec3(0, 1, 0));
  e.load_mesh("res/obj/floor.obj");
  e.load_mesh("lol.obj");

  camera cam = e.create_camera();
  e.set_current_camera(cam);

  cam->set_camera_position(glm::vec3(3, 3, 3));

  double a = 0;
  auto f = std::function<void()>([&](){ a-=0.0001; m->set_pos(glm::vec3(a, 0, 0)); });

  e.run(f);

  return 0;
}
