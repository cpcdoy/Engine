#include "utils.hh"

namespace util
{
    std::string replace_all(std::string str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
            elems.push_back(item);
        return elems;

    }

    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;
        split(s, delim, elems);
        for (unsigned int i = 0; i < elems.size(); i++)
            if (elems[i] == "" || elems[i] == " ")
            {
                elems.erase(elems.begin() + i);
                i = 0;

            }
        return elems;

    }

}
