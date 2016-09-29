#include "shader_translator.hh"

namespace render_backend
{
    template <>
        std::string shader_translator<opengl_shader_backend>::emit(std::string file)
        {
            pegtl::file_parser(file.c_str()).parse<rule_list, action_glsl, error_control>();

            auto shader = translator_state::emission_stream().get_shader_string();
            translator_state::emission_stream().clear();

            return shader;
        }
}
