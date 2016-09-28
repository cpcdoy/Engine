#pragma once

# include <iostream>
# include "shader_generator.hh"

namespace render_backend
{
    class opengl_shader_backend;

    template <typename Backend>
        class shader_parser
        {
            public:
                std::string emit() { std::cout << "Default\n";  }

        };


    /*template<enum shader_backend Backend, bool Enable = (Backend >= 0)>
        class shader_parser;

    template<enum shader_backend Backend>
        class shader_parser<Backend, shader_backend::glsl>
        {
            public:
                std::string emit() { std::cout << "lol1" << std::endl; }

            protected:
                enum shader_backend backend;
        };*/

}
