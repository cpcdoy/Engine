#include "camera.hh"

namespace scene
{
  camera::camera()
  {
  }

  camera::~camera()
  {
  }

  glm::mat4 camera::get_view_matrix()
  {
    return view;
  }

  glm::mat4 camera::get_projection_matrix()
  {
    return proj;
  }

  glm::vec3 camera::get_camera_position()
  {
    return pos;
  }

  void camera::set_camera_position(glm::vec3 v)
  {
    pos = v;
  }

  void camera::set_window_context(std::shared_ptr<ui::ui_manager> ui)
  {
		auto w = std::static_pointer_cast<ui::glfw_ui>(ui->get_ui())->get_window();
    this->window = w;
  }

  void camera::update(int w, int h)
  {
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int w_ = w / 2;
    int h_ = h / 2;
    glfwSetCursorPos(window, w_, h_);

    horizontal_angle += mouseSpeed * float(w_ - xpos);
    vertical_angle += mouseSpeed * float(h_ - ypos);

    glm::vec3 direction(cos(vertical_angle) * sin(horizontal_angle), sin(vertical_angle),
                        cos(vertical_angle) * cos(horizontal_angle));

    glm::vec3 right = glm::vec3(sin(horizontal_angle - 3.14f/2.0f), 0, cos(horizontal_angle - 3.14f/2.0f));

    glm::vec3 up = glm::cross( right, direction );

    if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS)
      pos += direction * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)
      pos -= direction * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)
      pos += right * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS)
      pos -= right * deltaTime * speed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      pos.y += deltaTime * speed;

    proj = glm::perspective(fov, (((float)w) / (float)(h)), 0.1f, 100.0f);
    view = glm::lookAt(pos, pos+direction, up);

    lastTime = currentTime;
  }
}
