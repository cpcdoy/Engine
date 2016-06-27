#pragma once

# include <GLFW/glfw3.h>
# include <glm/glm.hpp>
# include <glm/gtx/transform.hpp>
# include <glm/gtc/matrix_transform.hpp>

namespace scene
{
  class camera
  {
    public:
      camera();
      ~camera();

      void update();
      glm::mat4 get_view_matrix();
      glm::mat4 get_projection_matrix();
      glm::vec3 get_camera_position();
      void set_camera_position(glm::vec3);

      void set_window_context(GLFWwindow* window);

    private:
      GLFWwindow* window;

      glm::mat4 view;
      glm::mat4 proj;
      glm::vec3 pos;

      float horizontal_angle = -1.0f;
      float vertical_angle = 0.0f;
      float fov = 45.0f;

      float speed = 10.0f;
      float mouseSpeed = 0.005f;
  };
}
