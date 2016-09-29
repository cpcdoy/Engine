#pragma once
# include "shader_generator.hh"
# include "shader_grammar.hh"

namespace render_backend
{
    template <typename Rule>
        struct action_glsl : pegtl::nothing<Rule>
    {
    };

  template <>
  struct action_glsl<inline_comment>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
    }
  };
}
