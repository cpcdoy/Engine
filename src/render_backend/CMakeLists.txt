add_library(render_backend OBJECT opengl_backend.cc render_manager.cc
  opengl_shader_backend.cc opengl_backend_utils.cc
  opengl_shader_pass_no_lighting.cc opengl_shader_pass_geometry.cc
  opengl_shader_pass_ssao.cc opengl_shader_pass_shadow_map.cc
  opengl_shader_pass_lighting.cc texture_streamer.cc
  opengl_pipeline_state.cc shader_translator.cc glsl_shader_generator.cc)
