#include "../../src/engine/engine.hh"
#include <functional>

int main(void)
{
  engine::engine e;

  //Optional
  e.set_engine_render_context(render_backend::render_plugins::OpenGL, 4, 3);
  //Optional
  e.set_window_properties(1280, 720, "test");

  e.init();

  mesh m3 = e.load_mesh("res/obj/sponza.obj");

  mesh m4 = e.load_mesh("res/obj/california.obj");
  e.load_texture("res/tex/K.jpg", m4);
  m4->set_pos(glm::vec3(0, 0.15, 0));

  camera cam = e.create_camera();
  e.set_current_camera(cam);

  cam->set_camera_position(glm::vec3(3, 3, 3));

  double a = 0;
  auto f = std::function<void()>([&]()
      {
        a -= 0.0001;
      });

  e.run(f);

  return 0;
}
