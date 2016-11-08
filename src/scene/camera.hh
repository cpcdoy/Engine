#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif

# include <memory>
# include <glm/glm.hpp>
# include <glm/gtx/transform.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include "glm/gtx/string_cast.hpp"

# include "../ui/ui_manager.hh"

namespace scene
{
  /*class plane
  {
    public:
      plane()
      {
      }

      plane(glm::vec3 n, glm::vec3 c)
      {
        this->n = n;
        this->c = c;

        d = -glm::dot(n, c);
        std::cout << "d " << d << " " << glm::to_string(c) << std::endl;
      }

      float distance(glm::vec3 p)
      {
        return glm::dot(n, p) + d;
      }

    private:
      glm::vec3 c;
      glm::vec3 n;

      float d;
  }*/

  class camera
  {
    public:
      camera();
      ~camera();

      void update(int w, int h);

      glm::mat4 get_view_matrix();
      glm::mat4 get_projection_matrix();
      glm::vec3 get_camera_position();
      int get_fov();

      void compute_view_frustum_base();
      bool point_in_view(glm::vec3 p);

      void set_fov(int);
      void set_camera_position(glm::vec3);

      void set_window_context(std::shared_ptr<ui::ui_manager> ui);

    private:

      enum PLANE
      {
        NEAR,
        FAR,
        TOP,
        BOT,
        LEFT,
        RIGHT
      };

      GLFWwindow* window;

      glm::mat4 view;
      glm::mat4 proj;

      glm::vec3 pos;
      glm::vec3 direction;
      glm::vec3 right;
      glm::vec3 up;

      glm::vec3 norm_dir;
      glm::vec3 norm_right;
      glm::vec3 norm_up;

      float horizontal_angle = -1.0f;
      float vertical_angle = 0.0f;
      float fov = 45.0f;
      float tan_fov_2;

      float speed = 10.0f;
      float mouseSpeed = 0.005f;

      float near = 0.0001f;
      float far = 100.0f;

      float ratio = 16.0f / 9.0f;

      //std::array<plane, 6> frustum_planes;
  };
}
