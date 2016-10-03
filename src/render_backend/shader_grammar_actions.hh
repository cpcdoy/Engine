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
        auto comment = in.string();
        translator_state::emission_stream() << "// GLSL comment : " << comment;
    }
  };

  template <>
  struct action_glsl<version>
  {
    static void apply(const pegtl::action_input& in)
    {
        auto comment = in.string();
        translator_state::emission_stream() << "backend version";
    }
  };

  template <>
  struct action_glsl<backend_version>
  {
    static void apply(const pegtl::action_input& in)
    {
        auto comment = in.string();
        translator_state::emission_stream() << "ver";
    }
  };

  template <>
  struct action_glsl<backend_conf>
  {
    static void apply(const pegtl::action_input& in)
    {
        auto comment = in.string();
        translator_state::emission_stream() << "330";
    }
  };
}
