#include "opengl_backend.hh"

namespace render_backend
{
  opengl_backend::opengl_backend()
  {
    backend_id = "OpenGL Backend";

    //add_state<glm::vec3>("clear_color", glm::vec3(1.0, 1.0, 1.0));
  }

  opengl_backend::~opengl_backend()
  {
    debug::log::get(debug::logINFO) << "Shutting down the OpenGL render backend" << std::endl;
    glDeleteVertexArrays(vaos.size(), (const GLuint*)&vaos);
  }

  bool opengl_backend::init_backend(int w, int h)
  {
    glewExperimental = true;
    if (glewInit() != GLEW_OK) 
    {
      debug::log::get(debug::logERROR) << "Failed to initialize GLEW" << std::endl;
      debug::log::get(debug::logINDENT) << "You need to call the ui_manager, initialize it and create a window before" << std::endl;
      return false;
    }

		if (!check_gl_extensions())
      return false;

    add_state("width", w);
    add_state("height", h);
    add_state("shadow_map_res", 1024);

    opengl_pipeline_state::instance().get_state_of("width");

    //pipeline.push_back(std::make_shared<opengl_shader_pass_no_lighting>("res/shaders/no_lighting.vs", "res/shaders/no_lighting.fs"));
    pipeline.push_back(std::make_shared<opengl_shader_pass_shadow_map>("res/shaders/shadow_map.vs", "res/shaders/shadow_map.fs"));
    pipeline.push_back(std::make_shared<opengl_shader_pass_geometry>("res/shaders/geometry.vs", "res/shaders/geometry.fs"));
    pipeline.push_back(std::make_shared<opengl_shader_pass_ssao>("res/shaders/SSAO.vs", "res/shaders/SSAO.fs"));
    pipeline.push_back(std::make_shared<opengl_shader_pass_lighting>("res/shaders/lighting.vs", "res/shaders/lighting.fs"));

    glGenVertexArrays(1, &base_vao);
    glBindVertexArray(base_vao);

    auto sky = glm::vec3(0.0, 0.2, 0.4);
    glClearColor(sky.x, sky.y, sky.z, 1.0f);
    glClearDepth(1.0);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_FRAMEBUFFER_SRGB);
    
    return true;
  }

  void opengl_backend::set_clear_color(float r, float g, float b)
  {
    //add_state<glm::vec3>("clear_color", glm::vec3(r, g, b));
  }

  std::shared_ptr<resource::mesh>
  opengl_backend::generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh)
  {
    std::shared_ptr<resource::gl_mesh> gl_mesh = std::make_shared<resource::gl_mesh>(mesh);
    gl_mesh->set_vao(generate_vao(gl_mesh));

    return gl_mesh;
  }

  void opengl_backend::set_compatible_texture(std::shared_ptr<resource::mesh>& mesh, unsigned char* tex, int width, int height, texture_kind k)
  {
    debug::log::get(debug::logINFO) << "Generating a texture" << std::endl;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    glGenerateMipmap(GL_TEXTURE_2D);

    auto gl_mesh = std::static_pointer_cast<resource::gl_mesh>(mesh);
    if (k == texture_kind::ALBEDO)
      gl_mesh->set_texture(texture);
    else if (k == texture_kind::NORMAL)
      gl_mesh->set_normal_texture(texture);
    else if (k == texture_kind::METALNESS)
      gl_mesh->set_metalness_texture(texture);
    else if (k == texture_kind::ROUGHNESS)
      gl_mesh->set_roughness_texture(texture);
    else if (k == texture_kind::AO)
      gl_mesh->set_ao_texture(texture);

    debug::log::get(debug::logINDENT, 5) << "tex : " << texture << std::endl;
  }

  void opengl_backend::set_ui_manager(std::shared_ptr<ui::ui_manager> ui)
  {
    this->ui = ui;
  }

  GLuint opengl_backend::generate_vao(std::shared_ptr<resource::gl_mesh> mesh)
  {
    debug::log::get(debug::logINFO) << "Generating a VAO" << std::endl;

    GLuint vao;
    GLuint vertices_vbo, uvs_vbo, normals_vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Vertices
    glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->get_vertices().size() * sizeof(glm::vec3), &mesh->get_vertices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    //UVs
    glGenBuffers(1, &uvs_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->get_uvs().size() * sizeof(glm::vec3), &mesh->get_uvs()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);

    //Normals
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->get_normals().size() * sizeof(glm::vec3), &mesh->get_normals()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindVertexArray(base_vao);

    vaos.push_back(vao);

    debug::log::get(debug::logINDENT, 5) << "VAO : " << vao << std::endl;

    return vao;
  }

  void opengl_backend::batch(std::shared_ptr<scene::scene_manager> sm)
  {
    for (auto m : sm->get_render_queue())
      render_queue.push_back(std::static_pointer_cast<resource::gl_mesh>(m));

    debug::log::get(debug::logINDENT, 5) << "Batched " << sm->get_render_queue().size() << " meshes" << std::endl;
  }

  void opengl_backend::render()
  {
		static double elapsedTIme = 0;
    static int nb_frames = 0;
    nb_frames++;

		double currentTime = glfwGetTime();
    for (auto pass : pipeline)
      pass->process_pass(render_queue, cam);

		if (currentTime - elapsedTIme >= 1.0)
		{
      double ms = 1000.0 / double(nb_frames);
      std::cout << ms << " ms --> "
                << nb_frames << " fps" << std::endl;
			nb_frames = 0;
			elapsedTIme += 1.0;
		}
  }

  void opengl_backend::update_renderer()
  {
    cam->update(opengl_pipeline_state::instance().get_state_of("width"),
                opengl_pipeline_state::instance().get_state_of("height"));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void opengl_backend::add_state(std::string s, long r)
  {
    opengl_pipeline_state::instance().add_state(s, r);
  }
}
