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

    template<enum shader_backend Backend>
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

                template<enum shader_backend T>
                    using enable = typename std::enable_if<Backend == T>::type;

                template<typename T, typename = void>
                    struct pragma_options;

                template<typename T>
                    struct pragma_options<T, enable<shader_backend::glsl>>
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

                std::string get_profile(enum profile profile)
                {
                    static std::map<enum profile, std::string> profiles;
                    if (profiles.empty())
                    {
                        profiles.insert(std::make_pair(none, ""));
                        profiles.insert(std::make_pair(core, "core"));
                        profiles.insert(std::make_pair(profile, "profile"));
                    }

                    return profiles[profile];
                }

                //Special operations
                void set_version(int version, enum profile profile)
                {
                    gen << "#version " << version << space << get_profile(profile) << std::endl;
                }

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

                void dump()
                {
                    std::cout << "TEST" << std::endl;
                    std::cout << gen.str() << std::endl;
                }

                std::string get_shader()
                {
                    return gen.str();
                }

            protected:
                std::stringstream gen;
                const char space = ' ';
        };
}
