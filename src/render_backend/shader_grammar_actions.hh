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
    static void apply(const pegtl::action_input& in)
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
      static std::string infer_type(std::string value)
      {
          std::istringstream stream(value);
          float f;
          stream >> std::noskipws >> f;

          if (!stream.fail())
              return "float";
          return "error_type";
      }

    static void apply(const pegtl::action_input& in)
    {
        auto pack = translator_state::emission_stream().unpack();
        if (pack.size() == 1)
            translator_state::emission_stream().get_stream() << shader_generator<opengl_shader_backend>::generate_vardec("var", pack[0], "");
        else if (pack.size() > 1)
            translator_state::emission_stream().get_stream() << shader_generator<opengl_shader_backend>::generate_vardec(infer_type(pack[0]), pack[1], pack[0]);
    }
  };

  template <>
  struct action_glsl<vardec_value_float>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
    }
  };

  template <>
  struct action_glsl<vardec_value_int>
  {
    static void apply(const pegtl::action_input& in)
    {
        translator_state::emission_stream() << in.string();
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
