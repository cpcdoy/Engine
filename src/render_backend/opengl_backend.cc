#include "opengl_backend.hh"

namespace render_backend
{
  opengl_backend::opengl_backend()
  {
    backend_id = "OpenGL Backend";
  }

  opengl_backend::~opengl_backend()
  {
    debug::log::get(debug::logINFO) << "Shutting down the OpenGL render backend" << std::endl;
    glDeleteVertexArrays(vaos.size(), (const GLuint*)&vaos);
  }

  bool opengl_backend::init_backend()
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

    programs.push_back(load_shaders("res/shaders/geometry.vs", "res/shaders/geometry.fs"));

    glGenVertexArrays(1, &base_vao);
    glBindVertexArray(base_vao);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return true;
  }

  std::shared_ptr<resource::mesh>
  opengl_backend::generate_compatible_mesh(std::shared_ptr<resource::mesh> mesh)
  {
    std::shared_ptr<resource::gl_mesh> gl_mesh = std::make_shared<resource::gl_mesh>(mesh);

    gl_mesh->set_vao(generate_vao(gl_mesh));

    return gl_mesh;
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
      meshes.push_back(std::static_pointer_cast<resource::gl_mesh>(m));
  }

  void opengl_backend::render()
  {
    for (auto m : meshes)
    {
      glUseProgram(programs.back());

      glUniformMatrix4fv(glGetUniformLocation(programs.back(), "model"), 1, GL_FALSE, &m->get_model()[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(programs.back(), "projection"), 1, GL_FALSE, &cam->get_projection_matrix()[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(programs.back(), "view"), 1, GL_FALSE, &cam->get_view_matrix()[0][0]);

      glBindVertexArray(m->get_vao());
      glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());

      glBindVertexArray(base_vao);
    }
  }

  void opengl_backend::update_renderer()
  {
    cam->update();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}
