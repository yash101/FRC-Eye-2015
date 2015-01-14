#ifndef LOG_HPP
#define LOG_HPP
#include <string>
#include <fstream>
#include "../DevLib/Exceptions.hpp"
#include "../DevLib/misc.hpp"
namespace tool
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

    static ::tool::log Logger("log_main.txt");
}
#endif // LOG_HPP
