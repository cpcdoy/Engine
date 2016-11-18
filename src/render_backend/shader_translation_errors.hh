#pragma once

# include <sstream>
# include <iostream>

# include "../debug/log.hh"
# include "../utils/utils.hh"

# include "shader_grammar.hh"

namespace render_backend
{
    template <typename Rule>
        struct error_control_glsl : public pegtl::normal<Rule>
    {
        template <typename Input, typename... States>
            static void start(const Input&, States&&...)
            {
            }

        template <typename Input, typename... States>
            static void success(const Input&, States&&...)
            {
            }

        template <typename Input, typename... States>
            static void failure(const Input&, States&&...)
            {
            }

        static const std::string error_message();

        template <typename Input, typename... States>
            static void raise(const Input& in, States&&...);
    };

    template <typename T>
        template <typename Input, typename... States>
        void error_control_glsl<T>::raise(const Input& in, States&&...)
        {
            size_t char_pos = in.byte_in_line();

            std::ostringstream o;
            o << "An error occurred while parsing the shader \"";
            o << pegtl::position_info(in) << "\":\n    ";
            auto lines = util::split(in.begin() - char_pos, '\n');

            std::string line = lines.size() ? lines[0] : in.begin();

            o << line << std::endl;
            if (line.length() < 80)
            {
                for (size_t i = 0; i < char_pos + 4; i++)
                    o << " ";
                o << "\x1b[31m^\x1b[0m\n";
                if (char_pos + 4 > error_message().length() / 2)
                    for (size_t i = 0; i < char_pos + 4 - error_message().length() / 2; i++)
                        o << " ";
                else
                    for (size_t i = 0; i < char_pos + 4; i++)
                        o << " ";
                o << error_message();
            }

            debug::log::get(debug::logERROR) << o.str() << std::endl;
        }

    template <typename T>
        const std::string error_control_glsl<T>::error_message()
        {
            return "Error: " + pegtl::internal::demangle<T>();
        }

    template <>
        inline
        const std::string error_control_glsl<semicol>::error_message()
        {
            return "Missing semi-colon";
        }

    template <>
        inline
        const std::string error_control_glsl<vardec_name>::error_message()
        {
            return "Missing variable name";
        }

    template <>
        inline
        const std::string error_control_glsl<rules>::error_message()
        {
            return "Invalid construction/identifier";
        }

    template <>
        inline
        const std::string error_control_glsl<comment_sec_delim>::error_message()
        {
            return "Invalid comment... Missing another \"/\" (really ...?)";
        }

    template <>
        inline
        const std::string error_control_glsl<vardec_init>::error_message()
        {
            return "Missing initializer";
        }

    template <>
        inline
        const std::string error_control_glsl<backend_version>::error_message()
        {
            return "Missing/incorrect backend version. You need to enter a valid number (example: 330 for GLSL 330)";
        }
}
