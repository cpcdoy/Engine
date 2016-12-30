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
        translator_state::emission_stream().get_stream() << in.string();
    }
  };

  template <>
  struct action_glsl<version>
  {
    static void apply(const pegtl::action_input&)
    {
        auto pack = translator_state::emission_stream().unpack();
        translator_state::emission_stream().get_stream() << shader_generator<opengl_shader_backend>::generate_version(pack[0], shader_generator<opengl_shader_backend>::core);
    }
  };

  template <>
  struct action_glsl<backend_version>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
    }
  };

  template <>
  struct action_glsl<vardec>
  {
    static void apply(const pegtl::action_input&)
    {
        auto type_pack = translator_state::emission_stream().type_stream().unpack();
        auto pack = translator_state::emission_stream().unpack();
        auto size = pack.size();
        if (size == 1)
        {
            translator_state::emission_stream().get_stream() << shader_generator<opengl_shader_backend>::generate_vardec("to_be_infered", pack[0]);
            translator_state::emission_stream().add_vardec("", pack[0], "");
        }
        else if (size > 1)
        {
            translator_state::emission_stream().get_stream() << shader_generator<opengl_shader_backend>::generate_vardec(type_pack[0], pack[1], pack[0]);
            translator_state::emission_stream().add_vardec(type_pack[0], pack[1], pack[0]);
        }
    }
  };

  template <>
  struct action_glsl<vardec_value_float>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
        translator_state::emission_stream().type_stream() << "float";
    }
  };

  template <>
  struct action_glsl<vardec_value_int>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
        translator_state::emission_stream().type_stream() << "int";
    }
  };

  template <>
  struct action_glsl<vardec_name>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
    }
  };
}
