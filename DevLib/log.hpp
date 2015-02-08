#ifndef LOG_HPP
#define LOG_HPP
#include <string>
#include <fstream>
#include "misc.hpp"
#include "core.hpp"
namespace dev
{
    class log
    {
    private:
        std::string location;
        bool open();
        std::fstream fs;
    public:
        log(std::string location);
        ~log();
        template<typename T> void put(T x)
        {
            if(fs.is_open())
            {
                fs << "["<< dev::getTime() << "] -> \"" << x << "\"" << std::endl;
            }
        }
    };
    dev::log& getLogger();
}
#endif // LOG_HPP
