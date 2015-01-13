#ifndef HTTP_FUNCTIONS_HPP
#define HTTP_FUNCTIONS_HPP
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <map>

namespace dev
{
    inline std::string urldecode(std::string in)
    {
        std::string ret;
        for(unsigned int i = 0; i < in.size(); i++)
        {
            if ((int) in[i] == 37)
            {
                int new_char;
                sscanf(in.substr(i + 1, 2).c_str(), "%x", &new_char);
                char ch = static_cast<char>(new_char);
                ret += ch;
                i += 2;
            }
            else
            {
                ret += in[i];
            }
        }
        for(unsigned int i = 0; i < ret.size(); i++)
        {
            if(ret[i] == '+') { ret[i] = ' '; }
        }
        return ret;
    }

    inline std::string http_status(short code)
    {
        std::map<int, std::string> s;
        s[200] = "OK";
        s[201] = "Created";
        s[202] = "Accepted";
        s[204] = "No Content";
        s[300] = "Multiple Choices";
        s[301] = "Moved Permanently";
        s[302] = "Moved Temporarily";
        s[304] = "Not Modified";
        s[400] = "Bad Request";
        s[401] = "Unauthorized";
        s[403] = "Forbidden";
        s[404] = "Not Found";
        s[500] = "Internal Server Error";
        s[501] = "Not Implemented";
        s[502] = "Bad Gateway";
        s[503] = "Service Unavailable";

        if(s.find(code) == s.end())
        {
            return "Unknown Code";
        }
        return s[code];
    }
}
#endif
