#include "../logger.hpp"
#include "../misc.hpp"

//dev::Logger::Logger(std::string location)
//{
//    logOut.open(location.c_str(), std::ios::out);
//    CurrentMode = INFO;
//}
//
//template<typename DataType> void dev::Logger::put(DataType data, dev::LogType type)
//{
//    CurrentMode = type;
//    if(logOut.is_open())
//    {
//        logOut << dev::getTime() << "[" << dev::LogTypes[CurrentMode] << "] >> " << data;
//    }
//}
//
//template<typename DataType> void dev::Logger::put(DataType data)
//{
//    if(logOut.is_open())
//    {
//        logOut << dev::getTime() << "[" << dev::LogTypes[CurrentMode] << "] >> " << data;
//    }
//}