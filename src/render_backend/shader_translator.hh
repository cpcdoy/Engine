#pragma once

# include <iostream>
# include "shader_translator_state.hh"
# include "shader_generator.hh"
# include "shader_grammar.hh"
# include "shader_grammar_actions.hh"
# include "shader_translation_errors.hh"

namespace render_backend
{
    class opengl_shader_backend;

    template <typename Backend>
        class shader_translator
        {
            public:
                std::string emit(std::string file)
                {
                }

                std::string get_emission();
        };
}
