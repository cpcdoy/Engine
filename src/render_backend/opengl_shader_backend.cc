#include "opengl_backend.hh"
#include "shader_generator.hh"
#include "../utils/utils.hh"

#include "opengl_shader_backend.hh"
#include "../event/events.hh"

namespace render_backend
{
  opengl_shader_manager::opengl_shader_manager()
  {
    opengl_opt_ctx = glslopt_initialize(kGlslTargetOpenGL);

    shader_stages_info.insert(std::make_pair(vertex, stage_info { "vertex", GL_VERTEX_SHADER }));
    shader_stages_info.insert(std::make_pair(tcs, stage_info { "tesselation control", GL_TESS_CONTROL_SHADER }));
    shader_stages_info.insert(std::make_pair(tes, stage_info { "tesselation evaluation", GL_TESS_EVALUATION_SHADER }));
    shader_stages_info.insert(std::make_pair(geometry, stage_info { "geometry", GL_GEOMETRY_SHADER }));
    shader_stages_info.insert(std::make_pair(fragment, stage_info { "fragment", GL_FRAGMENT_SHADER }));
    shader_stages_info.insert(std::make_pair(compute, stage_info { "compute", GL_COMPUTE_SHADER }));
  }

  opengl_shader_manager::~opengl_shader_manager()
  {
    glUseProgram(0);
    for (const auto& id : shader_ids)
      glDeleteShader(id);
  }

  GLuint opengl_shader_manager::compile_shaders(std::string v_path,
      std::string f_path,
      std::string tcs_path,
      std::string tes_path,
      std::string g_path,
      std::string c_path)
  {
    compile_shader(v_path, vertex, true);
    compile_shader(tcs_path, tcs, false);
    compile_shader(tes_path, tes, false);
    compile_shader(g_path, geometry, false);
    compile_shader(f_path, fragment, true);
    compile_shader(c_path, compute, false);

    return link_program();
  }

  void opengl_shader_manager::set_state(enum shader_stage stage)
  {
    current_shader_type_str = shader_stages_info[stage].name;
    is_stage_optimizable = stage == vertex || stage == fragment;
    current_stage = shader_stages_info[stage].gl_id;
  }

  bool opengl_shader_manager::compile_shader(std::string path, enum shader_stage stage, bool optimize)
  {
    if (path == "")
      return false;

    set_state(stage);

    debug::log::get(debug::logINFO) << "Compiling " << current_shader_type_str << " shader : " << path << std::endl;
    glslopt_shader *shader_opt;
    GLuint shader_id = glCreateShader(current_stage);

    std::string shader_code;
    std::ifstream shader_stream(path, std::ios::in);

    if (shader_stream.is_open())
    {
      std::string Line = "";
      while(getline(shader_stream, Line))
        shader_code += "\n" + Line;
      shader_stream.close();

      if (optimize && is_stage_optimizable)
      {
        debug::log::get(debug::logINFO) << "Optimizing " << current_shader_type_str << " shader" << std::endl;
        if (stage == vertex)
          shader_opt = glslopt_optimize(opengl_opt_ctx, kGlslOptShaderVertex, shader_code.c_str(), 0);
        else
          shader_opt = glslopt_optimize(opengl_opt_ctx, kGlslOptShaderFragment, shader_code.c_str(), 0);

        opt_ok = glslopt_get_status(shader_opt);
      }
      if (is_stage_optimizable && opt_ok)
      {
        int stat_alu, stat_tex, stat_flow;
        glslopt_shader_get_stats(shader_opt, &stat_alu, &stat_tex, &stat_flow);
        debug::log::get(debug::logINDENT, 5 + 6) << "Stats: " << std::endl;
        debug::log::get(debug::logINDENT, 5 + 12) << stat_alu << " ALU, " << stat_tex << " tex, " << stat_flow << " flow" << std::endl;
        shader_code = glslopt_get_output(shader_opt);
      }
      else if (is_stage_optimizable)
      {
        debug::log::get(debug::logWARN) << "Could not optimize " << current_shader_type_str << " shader:" << std::endl;
        debug::log::get(debug::logINDENT, 5 + 6) << glslopt_get_log(shader_opt) << std::endl;
      }

      shader_code = util::replace_all(shader_code, "#version 140", version_and_opt);
    }
    else
    {
      debug::log::get(debug::logERROR) << "Impossible to open " << path << std::endl;
      debug::log::get(debug::logINDENT, 5) << "Are you in the correct directory ?" << std::endl;
      opengl_shader_backend_compilation_error = true;

      return false;
    }

    GLint result = GL_FALSE;
    int info_log_length;

    char const * source_pointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &source_pointer , NULL);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1)
    {
      std::vector<char> shader_error_msg(info_log_length + 1);
      glGetShaderInfoLog(shader_id, info_log_length, NULL, &shader_error_msg[0]);
      debug::log::get(debug::logERROR) << &shader_error_msg[0] << std::endl;
      opengl_shader_backend_compilation_error = true;
    }

    shader_ids[stage] = shader_id;

    return true;
  }

  GLuint opengl_shader_manager::link_program()
  {
    GLint result = GL_FALSE;
    int info_log_length;
    debug::log::get(debug::logINDENT, 5) << "Linking program" << std::endl;
    GLuint program_id = glCreateProgram();
    for (auto id : shader_ids)
      if (id)
        glAttachShader(program_id, id);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1)
    {
      std::vector<char> program_error_message(info_log_length + 1);
      glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
      debug::log::get(debug::logERROR) << &program_error_message[0] << std::endl;
      opengl_shader_backend_compilation_error = true;
    }

    for (auto& id : shader_ids)
    {
      if (id)
        glDeleteShader(id);
      id = 0;
    }

    if (opengl_shader_backend_compilation_error)
    {
      event::engine_stop_event event;
      event.error = true;
      event.circumstances = "Could not link shaders, see the above errors.";
      event::channel::broadcast(event);
    }

    programs.push_back(program_id);
    return program_id;
  }
}
