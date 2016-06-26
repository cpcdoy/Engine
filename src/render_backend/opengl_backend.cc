#include "opengl_backend.hh"

namespace render_backend
{
  opengl_backend::opengl_backend()
  {
    backend_id = "OpenGL Backend";
  }

  opengl_backend::~opengl_backend()
  {
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

    gl_mesh->set_vao(generate_vao());

    return gl_mesh;
  }

  GLuint opengl_backend::generate_vao()
  {
    debug::log::get(debug::logINFO) << "Generating a VAO" << std::endl;

    GLuint vao;
    glGenVertexArrays(1, &vao);

    debug::log::get(debug::logINDENT) << "VAO : " << vao << std::endl;

    return vao;
  }

  void opengl_backend::render(std::shared_ptr<resource::mesh> mesh)
  {
    auto m = std::static_pointer_cast<resource::gl_mesh>(mesh);
  }
}
