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
  e.load_texture("res/tex/dds/T_Brick_Yellow_D.dds", m3);
  e.load_texture("res/tex/dds/T_Brick_Yellow_M.dds", m3, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_R.dds", m3, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_AO.dds", m3, texture_kind::AO);

  mesh m4 = e.load_mesh("res/obj/california.obj");
  e.load_texture("res/tex/dds/fake_tex.dds", m4);
  e.load_texture("res/tex/dds/default_AO.dds", m4, texture_kind::AO);
  m4->set_pos(glm::vec3(0, 0.15, 0));

  mesh m6 = e.load_mesh("res/obj/nanosuit.obj");
  e.load_texture("res/tex/dds/fake_tex.dds", m6);
  e.load_texture("res/tex/dds/default_AO.dds", m6, texture_kind::AO);
  m6->set_pos(glm::vec3(2, 0.15, 0));
  m6->set_scale(glm::vec3(0.7));

  mesh m2 = e.load_mesh("res/obj/sphere.obj");
  m2->set_lod_distance(0, 100);
  e.add_mesh_lod("res/obj/sphere_lod_1.obj", m2, 200, 1);
  e.load_texture("res/tex/dds/wood_albedo.dds", m2);
  e.load_texture("res/tex/dds/wood_metalness.dds", m2, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/wood_roughness.dds", m2, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/default_AO.dds", m2, texture_kind::AO);
  m2->set_pos(glm::vec3(-0.1, 5, -1));

  mesh m1 = e.load_mesh("res/obj/sphere.obj");
  m1->set_lod_distance(0, 100);
  e.add_mesh_lod("res/obj/sphere_lod_1.obj", m1, 200, 1);
  e.load_texture("res/tex/dds/T_Tile_White_D.dds", m1);
  e.load_texture("res/tex/dds/T_Tile_White_M.dds", m1, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/T_Tile_White_R.dds", m1, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/T_Tile_White_AO.dds", m1, texture_kind::AO);
  m1->set_pos(glm::vec3(-0.1, 5, 1.3));

  mesh m5 = e.load_mesh("res/obj/sphere.obj");
  m5->set_lod_distance(0, 100);
  e.add_mesh_lod("res/obj/sphere_lod_1.obj", m5, 200, 1);
  e.load_texture("res/tex/dds/T_Brick_Yellow_D.dds", m5);
  e.load_texture("res/tex/dds/T_Brick_Yellow_M.dds", m5, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_R.dds", m5, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_AO.dds", m5, texture_kind::AO);
  m5->set_pos(glm::vec3(-0.1, 7, 1.3));

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
