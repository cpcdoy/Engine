#pragma once

# include <sstream>

# include "shader_grammar.hh"

namespace render_backend
{
    template <typename Rule>
        struct error_control : public pegtl::normal<Rule>
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
        void error_control<T>::raise(const Input& in, States&&...)
        {

        }
}
