#pragma once

# include <sstream>
# include <stack>
# include <vector>

namespace render_backend
{
    class elements_stream
    {
        public:
            elements_stream& operator<<(const std::string& str)
            {
                stack.push(str);
                return *this;
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

        protected:
            std::stack<std::string> stack;

    };

    struct variable_data
    {
      std::string type;
      std::string name;
      std::string value;
    };

    class translator_state : public elements_stream
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

            void add_vardec(std::string type, std::string name, std::string val = "")
            {
              variable_data var = { type, name, val };
              vars.push_back(var);
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

            elements_stream& type_stream()
            {
              return type_stream_;
            }

            void clear()
            {
              stream.str(std::string());
            }

        protected:
            std::stringstream stream;
            elements_stream type_stream_;
            std::vector<variable_data> vars;
    };
}
