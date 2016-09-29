#pragma once

# include <sstream>

namespace render_backend
{
    class translator_state
    {
        public:
            translator_state& operator<<(const std::string& str)
            {
                stream << str;
            }

            std::string get_shader_string()
            {
                return stream.str();
            }

            static translator_state& emission_stream()
            {
                static translator_state emission_stream;
                return emission_stream;
            }

            void clear()
            {
                stream.str(std::string());
            }

        protected:
            std::stringstream stream;
    };
}
