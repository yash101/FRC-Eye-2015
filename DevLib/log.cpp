#include "log.hpp"
#include "../buildconf.h"
#ifndef LOG_LOCATION
#define LOG_LOCATION "log.txt"
#endif
dev::log Logger(LOG_LOCATION);
dev::log& dev::getLogger() { return Logger; }

dev::log::log(std::string loc) : location(loc)
{
    if(!open())
    {
        throw dev::exception("Unable to open log!");
    }
    put("Log opened successfully for writing to.");
}

bool dev::log::open()
{
    fs.open(location.c_str(), std::ios::out | std::ios::app);
    return fs.is_open();
}

dev::log::~log()
{
    fs.close();
}
