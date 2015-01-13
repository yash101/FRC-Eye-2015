#include "../misc.hpp"
#include <ctime>
#include <sstream>
std::string dev::getTime()
{
    std::stringstream str;
    std::time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    str << asctime(timeinfo);

    if(str.str().size() != 0)
    {
        return str.str().substr(0, str.str().size() - 1);
    }
    else
    {
        return str.str();
    }
}

