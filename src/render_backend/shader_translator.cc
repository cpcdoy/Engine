#include "shader_translator.hh"

namespace render_backend
{
    template <>
        std::string shader_translator<opengl_shader_backend>::translate(std::string file)
        {
            pegtl::file_parser(file.c_str()).parse<rule_list, action_glsl, error_control_glsl>();

            auto shader = std::string(translator_state::emission_stream().get_shader_string());
            translator_state::emission_stream().clear();

            return shader;
        }
}
