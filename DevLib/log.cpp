#include "log.hpp"
tool::log::log(std::string loc) : location(loc)
{
    if(!open())
    {
        throw dev::exception("Unable to open log!");
    }
    put("Log opened successfully for writing to.");
}

bool tool::log::open()
{
    fs.open(location.c_str(), std::ios::out | std::ios::app);
    return fs.is_open();
}

tool::log::~log()
{
    fs.close();
}
