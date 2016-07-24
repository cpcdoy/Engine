#include "opengl_backend.hh"

namespace render_backend
{
  GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path)
  {
    bool opengl_shader_backend_compilation_error = false;
    debug::log::get(debug::logINFO) << "Compiling shaders" << std::endl;

    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
    if(vertex_shader_stream.is_open())
    {
      std::string Line = "";
      while(getline(vertex_shader_stream, Line))
        vertex_shader_code += "\n" + Line;
      vertex_shader_stream.close();
    }
    else
    {
      debug::log::get(debug::logERROR) << "Impossible to open " << vertex_file_path << std::endl;
      debug::log::get(debug::logINDENT) << "Are you in the correct directory ?" << std::endl;
      opengl_shader_backend_compilation_error = true;

      return 0;
    }

    std::string fragment_shader_code;
    std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
    if(fragment_shader_stream.is_open())
    {
      std::string Line = "";
      while(getline(fragment_shader_stream, Line))
        fragment_shader_code += "\n" + Line;
      fragment_shader_stream.close();
    }
    else
    {
      debug::log::get(debug::logERROR) << "Impossible to open " << fragment_file_path << std::endl;
      debug::log::get(debug::logINDENT) << "Are you in the correct directory ?" << std::endl;
      opengl_shader_backend_compilation_error = true;

      return 0;
    }

    GLint Result = GL_FALSE;
    int info_log_length;

    debug::log::get(debug::logINDENT, 5) << "Vertex shader: " << vertex_file_path << std::endl;
    char const * vertex_source_pointer = vertex_shader_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_source_pointer , NULL);
    glCompileShader(vertex_shader_id);

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1)
    {
      std::vector<char> vertex_shader_error_msg(info_log_length+1);
      glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_msg[0]);
      debug::log::get(debug::logERROR) << &vertex_shader_error_msg[0] << std::endl;
      opengl_shader_backend_compilation_error = true;
    }

    debug::log::get(debug::logREINDENT) << "Fragment shader: " << fragment_file_path << std::endl;
    char const * FragmentSourcePointer = fragment_shader_code.c_str();
    glShaderSource(fragment_shader_id, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1)
    {
      std::vector<char> fragment_shader_error_msg(info_log_length+1);
      glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_error_msg[0]);
      debug::log::get(debug::logERROR) << &fragment_shader_error_msg[0] << info_log_length << std::endl;
      opengl_shader_backend_compilation_error = true;
    }

    debug::log::get(debug::logREINDENT) << "Linking program" << std::endl;
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1)
    {
      std::vector<char> program_error_message(info_log_length+1);
      glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
      debug::log::get(debug::logERROR) << &program_error_message[0] << std::endl;
      opengl_shader_backend_compilation_error = true;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    assert(!opengl_shader_backend_compilation_error);

    return program_id;
  }
}
