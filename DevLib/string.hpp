#ifndef STRING_HPP
#define STRING_HPP
#include <string>
#include <cstring>
#include <sstream>

namespace dev
{
    template<typename C> inline std::string toString(C toStr)
    {
        std::stringstream str;
        str << toStr;
        return str.str();
    }

    inline bool equals(std::string a, std::string b) { return !strcmp(a.c_str(), b.c_str()); }

    inline int charPos(std::string in, char f)
    {
        for(unsigned int i = 0; i < in.size(); i++)
        {
            if(in[i] == f)
            {
                return i;
            }
        }
        return -1;
    }

    template<typename Type> void fromString(Type& x, std::string source)
    { std::stringstream str(source); str >> x; return; }

    inline bool contains(char find, std::string dict)
    {
        for(unsigned int i = 0; i < dict.size(); i++)
        {
            if(dict[i] == find)
            {
                return true;
            }
        }
        return false;
    }

    inline void trim(std::string& in)
    {
        size_t i = 0;
        while(std::isspace(in[i]) && i <= in.size()) i++;
        in = in.substr(i, in.size());

        i = in.size();
        while(std::isspace(in[i]) && i != 0) i--;
        in = in.substr(0, i);
    }

    inline std::string itrim(std::string in)
    {
        std::string x = in;
        dev::trim(x);
        return x;
    }

    inline std::string tolower(std::string in)
    {
        std::string o;
        for(unsigned int i = 0; i < in.size(); i++)
        {
            o += std::tolower(in[i]);
        }
        return o;
    }

    inline bool getline(std::string& buffer, std::string end, std::istream& stream)
    {
        buffer.clear();
        std::string out;
        while(stream.good())
        {
            if(out.size() >= end.size())
            {
                if(out.substr(out.size() - end.size(), out.size()) == end)
                {
                    buffer = out.substr(0, out.size() - end.size());
                    return true;
                }
            }
            out += (char) stream.get();
        }
        buffer = true;
        return false;
    }

    inline int stoi(std::string str) { return std::atoi(str.c_str()); }
    inline float stof(std::string str) { return std::atof(str.c_str()); }
    inline long stol(std::string str) { return std::stol(str.c_str()); }
    inline long long stoll(std::string str) { return std::atoll(str.c_str()); }
    template<typename T> inline T ston(std::string in) { T x; std::stringstream(in) >> x; return x; }
}

inline bool operator==(std::string a, std::string b) { return !strcmp(a.c_str(), b.c_str()); }
inline bool operator!=(std::string a, std::string b) { return strcmp(a.c_str(), b.c_str()); }
#endif // STRING_HPP
