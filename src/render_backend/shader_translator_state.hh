#pragma once

# include <sstream>
# include <stack>
# include <vector>
# include <map>

namespace render_backend
{
    class translator_state
    {
        public:
            std::stringstream& operator<<(const std::string& str)
            {
                stack.push(str);
                return stream;
            }

            std::stringstream& get_stream()
            {
                return stream;
            }

            void add_vardec(std::string var)
            {
                vars.insert(std::make_pair(var, ""));
            }

            std::vector<std::string> unpack()
            {
                std::vector<std::string> pack;
                while (!stack.empty())
                {
                    pack.push_back(stack.top());
                    stack.pop();
                }

                return pack;
            }

            std::string get_shader_string()
            {
                return std::string(stream.str());
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
            std::stack<std::string> stack;
            std::map<std::string, std::string> vars;
    };
}
