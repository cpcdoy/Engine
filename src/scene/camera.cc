#include "camera.hh"

namespace scene
{
  camera::camera()
  {
    proj = glm::perspective(fov, ratio, near, far);
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

  int camera::get_fov()
  {
    return fov;
  }

  void camera::set_fov(int f)
  {
    fov = f;
  }

  void camera::compute_view_frustum_base()
  {
    /*constexpr float ang_to_rad = 3.14159265358979323846f / 180.0f;

    float tan_fov_2 = tan(ang_to_rad * fov * 0.5f);
    float h_near = tan_fov_2 * near;
    float w_near = h_near * ratio;

    float h_far = tan_fov_2 * far;
    float w_far = h_far * ratio;*/

    //Normalize direction base vectors
    norm_dir = glm::normalize(direction);
    norm_right = glm::normalize(glm::cross(norm_dir, up));
    norm_up = glm::cross(norm_right, norm_dir);

    constexpr float ang_to_rad = 3.14159265358979323846f / 180.0f;

    tan_fov_2 = 2.0f * tan(ang_to_rad * fov * 0.5f);

    /*float h_far_2 = h_far / 2.0f;
    float w_far_2 = w_far / 2.0f;*/

    //Far center point
    /*glm::vec3 far_center = pos - norm_dir * far;

    glm::vec3 norm_up_h_far_2 = norm_up * h_far_2;
    glm::vec3 norm_right_w_far_2 = norm_right * w_far_2;
    //Far plane points
    glm::vec3 far_top_left = far_center + norm_up_h_far_2 - norm_right_w_far_2;
    glm::vec3 far_top_right = far_center + norm_up_h_far_2 + norm_right_w_far_2;
    glm::vec3 far_bot_left = far_center - norm_up_h_far_2 - norm_right_w_far_2;
    glm::vec3 far_bot_right = far_center - norm_up_h_far_2 + norm_right_w_far_2;

    float h_near_2 = h_near / 2.0f;
    float w_near_2 = w_near / 2.0f;

    //Near center point
    glm::vec3 near_center = pos - norm_dir * near;

    glm::vec3 norm_up_h_near_2 = norm_up * h_near_2;
    glm::vec3 norm_right_w_near_2 = norm_right * w_near_2;
    //Near plane points
    glm::vec3 near_top_left = near_center + norm_up_h_near_2 - norm_right_w_near_2;
    glm::vec3 near_top_right = near_center + norm_up_h_near_2 + norm_right_w_near_2;
    glm::vec3 near_bot_left = near_center - norm_up_h_near_2 - norm_right_w_near_2;
    glm::vec3 near_bot_right = near_center - norm_up_h_near_2 + norm_right_w_near_2;*/

    //Describe the view frustum planes by a point and a normal
    /*glm::vec3 c, n;

    frustum_planes[NEAR] = plane(-norm_dir, near_center);
    frustum_planes[FAR] = plane(norm_dir, far_center);

    std::cout << " norm up " << glm::to_string(norm_up) << std::endl;
    c = glm::normalize((near_center + norm_up * h_near) - pos);
    n = c * norm_right;
    frustum_planes[TOP] = plane(n, near_center + norm_up * h_near);

    c = glm::normalize((near_center - norm_up * h_near) - pos);
    n = norm_right * c;
    frustum_planes[BOT] = plane(n, near_center - norm_up * h_near);

    c = glm::normalize((near_center - norm_right * w_near) - pos);
    n = c * norm_up;
    frustum_planes[LEFT] = plane(n, near_center - norm_right * w_near);

    c = glm::normalize((near_center + norm_right * w_near) - pos);
    n = norm_up * c;
    frustum_planes[RIGHT] = plane(n, near_center + norm_right * w_near);*/
  }

  bool camera::point_in_view(glm::vec3 p)
  {
    float pcx, pcy, pcz, aux;

    glm::vec3 v = p - pos;

    pcz = glm::dot(v, norm_dir);
    if (pcz > far || pcz < near)
      return false;

    pcy = glm::dot(v, norm_up);
    aux = pcz * tan_fov_2;
    if (pcy > aux || pcy < -aux)
      return false;

    pcx = glm::dot(v, norm_right);
    aux *= ratio;
    if (pcx > aux || pcx < -aux)
      return false;

    return true;
  }

  void camera::set_window_context(std::shared_ptr<ui::ui_manager> ui)
  {
    auto w = std::static_pointer_cast<ui::glfw_ui>(ui->get_ui())->get_window();
    this->window = w;
  }

  void camera::update(int w, int h)
  {
    static double lastTime = glfwGetTime();
    ratio = (((float)w) / (float)(h));

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int w_2 = w / 2;
    int h_2 = h / 2;
    glfwSetCursorPos(window, w_2, h_2);

    horizontal_angle += mouseSpeed * float(w_2 - xpos);
    vertical_angle += mouseSpeed * float(h_2 - ypos);

    direction = glm::vec3(cos(vertical_angle) * sin(horizontal_angle), sin(vertical_angle),
                        cos(vertical_angle) * cos(horizontal_angle));

    right = glm::vec3(sin(horizontal_angle - 3.14f/2.0f), 0, cos(horizontal_angle - 3.14f/2.0f));

    up = glm::cross(right, direction);

    if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS)
      pos += direction * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)
      pos -= direction * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)
      pos += right * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS)
      pos -= right * deltaTime * speed;
    if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
      pos += direction * deltaTime * speed * 3.0f;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      pos.y += deltaTime * speed;

    view = glm::lookAt(pos, pos + direction, up);

    lastTime = currentTime;

    compute_view_frustum_base();
  }
}
