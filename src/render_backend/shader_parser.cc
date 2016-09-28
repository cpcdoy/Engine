#include "shader_parser.hh"

namespace render_backend
{
    template <>
        std::string shader_parser<opengl_shader_backend>::emit()
        {
            std::cout << "glsl backend\n";
        }
}
