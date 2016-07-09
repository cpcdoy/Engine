add_library(render_backend OBJECT opengl_backend.cc render_manager.cc
  opengl_shader_backend.cc opengl_backend_utils.cc
  opengl_shader_pass_no_lighting.cc opengl_shader_pass_geometry.cc
  opengl_shader_pass_ssao.cc)
