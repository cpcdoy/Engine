#pragma once

# include <map>
# include <array>

# include "../../3rd_party/glsl-optimizer/src/glsl/glsl_optimizer.h"

namespace render_backend
{
  enum shader_stage
  {
    vertex,
    tcs,
    tes,
    geometry,
    fragment,
    compute,
    all
  };

  class opengl_shader_manager
  {
    public:
      opengl_shader_manager();
      ~opengl_shader_manager();
      GLuint compile_shaders(std::string v_path,
                             std::string f_path,
                             std::string tcs_path = "",
                             std::string tes_path = "",
                             std::string g_path = "",
                             std::string c_path = "");
      bool compile_shader(std::string path, enum shader_stage stage, bool optimize);
      GLuint link_program();

      struct stage_info
      {
        std::string name;
        GLuint gl_id;
      };

    private:
      void set_state(enum shader_stage stage);

      const char *version_and_opt = "#version 330\n"
        "#pragma optionNV(fastmath on)\n"
        "#pragma optionNV(fastprecision on)\n"
        "#pragma optionNV(ifcvt none)\n"
        "#pragma optionNV(inline all)\n"
        "#pragma optionNV(strict on)\n"
        "#pragma optionNV(unroll all)\n";

      glslopt_ctx *opengl_opt_ctx = nullptr;

      bool opt_ok;

      std::string current_shader_type_str;
      bool is_stage_optimizable = false;
      GLuint current_stage;

      bool opengl_shader_backend_compilation_error = false;

      std::map<enum shader_stage, stage_info> shader_stages_info;

      std::array<GLuint, all> shader_ids;

      std::vector<GLuint> programs;
  };
}
