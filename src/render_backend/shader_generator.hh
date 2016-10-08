#pragma once
# include <type_traits>
# include <map>
# include <sstream>

namespace render_backend
{
    enum shader_backend
    {
        glsl,
        hlsl
    };

    class opengl_shader_backend;

    const char space = ' ';
    const char new_line = '\n';
    const char semi_col = ';';

    template<typename Backend>
        class shader_generator
        {
            public:
                enum profile
                {
                    none,
                    core,
                    profile,
                    all
                };

                enum swizzle
                {
                    x,
                    y,
                    z,
                    w,
                    r,
                    g,
                    b,
                    a,
                    s,
                    t,
                    u,
                    v
                };

                template<typename T>
                    struct pragma_options
                    {
                        pragma_options(T opt, T par, T val)
                            : option(opt), param(par), value(val)
                        {
                        }

                        T option;
                        T param;
                        T value;
                    };

                template<typename T>
                    struct define_options
                    {
                        define_options(T n, T val)
                            : name(n), rvalue(val)
                        {
                        }

                        T name;
                        T rvalue;
                    };

                static std::string get_profile(enum profile profile);


                //Special operations
                static std::string generate_version(std::string version, enum profile profile);

                //Vardec with optional value
                static std::string generate_vardec(std::string type, std::string name, std::string value);

                template<typename... Options>
                    void pragma(Options... options)
                    {
                        //static_assert(util::all_same<pragma_options, Options...>::value, "pragma's parameters must be of the same type");

                        for (const auto& opt : {options...})
                            gen << "#pragma " << opt.option << '(' << opt.param << space << opt.value << ')' << std::endl;
                    }

                template<typename... Defines>
                    void define(Defines... defines)
                    {
                        for (const auto& def : {defines...})
                            gen << "#define " << def.name << space << def.rvalue << std::endl;
                    }

                //Declarations
                template<typename Type,
                    typename... Qualifiers>
                        void declare_variable(Type type, std::string name, Qualifiers... qualifiers) {}
                template<typename Type,
                    typename... ArgsTypes,
                    typename... Args>
                        void declare_function(Type type, ArgsTypes... args_types, Args... args) {}

                //Call
                template<typename Function, typename... Args>
                    void function_call(Function function, Args... args)
                    {
                        gen << function << '(';

                        auto pack_size = sizeof...(Args);
                        auto index = 0;
                        auto pack = {args...};
                        for (auto i = pack.begin(); i < pack.end(); i++, index++)
                            gen << *i << (index == pack_size - 1 ? "" : ", ");

                        gen << ");";
                    }

                template<typename Type>
                    void rvalue(Type rvalue) {}

                /*template<typename Type>
                  void ternary(Type type, ) {}*/

                template<typename Type>
                    void assign(Type rvalue1, Type rvalue2) {}

                //Operations
                //Arith
                template<typename... Type>
                    void add(Type... rvalues) {}
                template<typename... Type>
                    void sub(Type... rvalues) {}
                template<typename... Type>
                    void mul(Type... rvalues) {}
                template<typename... Type>
                    void div(Type... rvalues) {}
                template<typename... Type>
                    void mod(Type... rvalues) {}
                template<typename... Type>
                    void inc(Type... rvalues) {}
                template<typename... Type>
                    void dec(Type... rvalues) {}
                template<typename... Type,
                    typename... Components>
                        void swizzle(Type... rvalue, Components... components) {}
                //Logical
                template<typename Type>
                    void land() {}
                template<typename Type>
                    void lor() {}
                template<typename Type>
                    void lxor() {}
                //Comparaison
                template<typename Type>
                    void cand() {}
                template<typename Type>
                    void cor() {}
                template<typename Type>
                    void cequal() {}
                template<typename Type>
                    void clequal() {}
                template<typename Type>
                    void cgequal() {}
                template<typename Type>
                    void cnot_equal() {}

                //Qualifier
                void add_qualifier() {}

                //Scopes
                void scope_begin() {}

                void scope_end() {}

            protected:
                std::stringstream gen;
        };
}
