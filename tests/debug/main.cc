#include "../../src/engine/engine.hh"
#include <functional>

int main(void)
{
  engine::engine e;

  //Remove debug output (EXPERIMENTAL)
  //debug::log::get(debug::logDEBUG, 0, false);
  //Optional
  e.set_engine_render_context(render_backend::render_plugins::OpenGL, 4, 3);
  //Optional
  e.set_window_properties(1280, 720, "test");

  e.init();

  mesh m3 = e.load_mesh("res/obj/sponza.obj");
  e.load_texture("res/tex/dds/T_Brick_Yellow_D.dds", m3);
  e.load_texture("res/tex/dds/T_Brick_Yellow_M.dds", m3, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_R.dds", m3, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_N.dds", m3, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/T_Brick_Yellow_AO.dds", m3, texture_kind::AO);

  mesh m4 = e.load_mesh("res/obj/sphere_s.obj");
  e.load_texture("res/tex/dds/rock_D.dds", m4);
  e.load_texture("res/tex/dds/rock_AO.dds", m4, texture_kind::AO);
  e.load_texture("res/tex/dds/rock_N.dds", m4, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/rock_R.dds", m4, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/rock_DI.dds", m4, texture_kind::DISPLACEMENT);
  m4->set_scale(glm::vec3(1.2, 1, 1));
  m4->set_pos(glm::vec3(0, 1, -0.2));

  mesh m8 = e.load_mesh("res/obj/sphere_s.obj");
  e.load_texture("res/tex/dds/block_D.dds", m8);
  e.load_texture("res/tex/dds/block_AO.dds", m8, texture_kind::AO);
  e.load_texture("res/tex/dds/block_N.dds", m8, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/block_R.dds", m8, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/block_DI.dds", m8, texture_kind::DISPLACEMENT);
  m8->set_pos(glm::vec3(0, 1, 2));

  mesh m10 = e.load_mesh("res/obj/sphere_s.obj");
  e.load_texture("res/tex/dds/greasy_D.dds", m10);
  e.load_texture("res/tex/dds/default_AO.dds", m10, texture_kind::AO);
  e.load_texture("res/tex/dds/greasy_N.dds", m10, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/greasy_R.dds", m10, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/greasy_M.dds", m10, texture_kind::METALNESS);
  m10->set_pos(glm::vec3(-4, 1, 0));

  /*mesh m7 = e.load_mesh("res/obj/nanosuit.obj");
  e.load_texture("res/tex/dds/fake_tex.dds", m7);
  e.load_texture("res/tex/dds/default_AO.dds", m7, texture_kind::AO);
  m7->set_pos(glm::vec3(2, 0.15, 0));
  m7->set_scale(glm::vec3(0.7));*/

  mesh m2 = e.load_mesh("res/obj/sphere_s.obj");
  m2->set_lod_distance(0, 100);
  e.load_texture("res/tex/dds/wood_albedo.dds", m2);
  e.load_texture("res/tex/dds/wood_metalness.dds", m2, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/wood_roughness.dds", m2, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/wood_N.dds", m2, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/wood_DI.dds", m2, texture_kind::DISPLACEMENT);
  e.load_texture("res/tex/dds/default_AO.dds", m2, texture_kind::AO);
  m2->set_pos(glm::vec3(-4, 1, 2));

  /*mesh m9 = e.load_mesh("res/obj/Objects_001.obj");
  m9->set_lod_distance(0, 100);
  e.load_texture("res/tex/dds/bark_D.dds", m9);
  e.load_texture("res/tex/dds/bark_M.dds", m9, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/bark_R.dds", m9, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/default_AO.dds", m9, texture_kind::AO);
  m9->set_pos(glm::vec3(4.5, 0.15, 0));
  m9->set_scale(glm::vec3(0.05));*/

  mesh m1 = e.load_mesh("res/obj/sphere_s.obj");
  m1->set_lod_distance(0, 100);
  e.load_texture("res/tex/dds/T_Tile_White_D.dds", m1);
  e.load_texture("res/tex/dds/T_Tile_White_M.dds", m1, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/T_Tile_White_R.dds", m1, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/T_Tile_White_N.dds", m1, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/T_Tile_White_DI.dds", m1, texture_kind::DISPLACEMENT);
  e.load_texture("res/tex/dds/T_Tile_White_AO.dds", m1, texture_kind::AO);
  m1->set_pos(glm::vec3(-8, 1, 0));

  mesh m5 = e.load_mesh("res/obj/sphere_s.obj");
  m5->set_lod_distance(0, 100);
  e.load_texture("res/tex/dds/T_Brick_Yellow_D.dds", m5);
  e.load_texture("res/tex/dds/T_Brick_Yellow_M.dds", m5, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_R.dds", m5, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/T_Brick_Yellow_N.dds", m5, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/T_Brick_Yellow_DI.dds", m5, texture_kind::DISPLACEMENT);
  e.load_texture("res/tex/dds/T_Brick_Yellow_AO.dds", m5, texture_kind::AO);
  m5->set_pos(glm::vec3(-8, 1, 2));

  mesh m11 = e.load_mesh("res/obj/sphere_s.obj");
  m11->set_lod_distance(0, 100);
  e.load_texture("res/tex/dds/rust_D.dds", m11);
  e.load_texture("res/tex/dds/rust_M.dds", m11, texture_kind::METALNESS);
  e.load_texture("res/tex/dds/rust_R.dds", m11, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/rust_N.dds", m11, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/default_AO.dds", m11, texture_kind::AO);
  m11->set_pos(glm::vec3(3, 1, 0));

  mesh m12 = e.load_mesh("res/obj/sphere_s.obj");
  e.load_texture("res/tex/dds/rough_rock_D.dds", m12);
  e.load_texture("res/tex/dds/rough_rock_R.dds", m12, texture_kind::ROUGHNESS);
  e.load_texture("res/tex/dds/rough_rock_N.dds", m12, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/rough_rock_DI.dds", m12, texture_kind::DISPLACEMENT);
  e.load_texture("res/tex/dds/rough_rock_AO.dds", m12, texture_kind::AO);
  m12->set_pos(glm::vec3(3, 1, 2));

  mesh m13 = e.load_mesh("res/obj/sphere_s.obj");
  e.load_texture("res/tex/dds/fake_tex.dds", m13);
  e.load_texture("res/tex/dds/default_N.dds", m13, texture_kind::NORMAL);
  e.load_texture("res/tex/dds/default_AO.dds", m13, texture_kind::AO);
  m13->set_pos(glm::vec3(5, 1, 0));

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
