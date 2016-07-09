#pragma once

# ifndef GLEW_HEADER
#  define GLEW_HEADER
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
# endif

namespace render_backend
{
  GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path);
  float lerp(float v0, float v1, float t);
}
