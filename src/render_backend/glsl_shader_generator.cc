#include "shader_generator.hh"

namespace render_backend
{
    template<>
        std::string shader_generator<opengl_shader_backend>::get_profile(enum profile profile)
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

    template<>
        std::string shader_generator<opengl_shader_backend>::generate_version(std::string version, enum profile profile)
        {
            std::stringstream stream;
            stream << "#version " << version << space << shader_generator<opengl_shader_backend>().get_profile(profile) << new_line;
            return stream.str();
        }

    template<>
        std::string shader_generator<opengl_shader_backend>::generate_vardec(std::string type, std::string name, std::string value)
        {
            std::stringstream stream;
            stream << type << space << name;
            if (value.length() > 0)
                stream << space << "=" << space << value;
            stream << semi_col << new_line;

            return stream.str();
        }
}
