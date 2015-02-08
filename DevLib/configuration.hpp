#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <string>
#include <map>
namespace dev
{
    class ConfigurationParser
    {
    private:
        std::map<std::string, std::string> _cfdata;
        std::string _location;
    public:
        ConfigurationParser(std::string location);
        std::string& operator[](std::string key);
        void remove(std::string key);
        std::string get(std::string key);
        void add(std::string key, std::string value);
        void refresh();
        void flush();
        std::map<std::string, std::string> getMap() { return _cfdata; }
        bool tryBool(std::string key);
        std::string getStorageLocation() { return _location; }
    };
}
#endif // CONFIGURATION_HPP
