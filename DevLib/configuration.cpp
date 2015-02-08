#include "configuration.hpp"
#include "string.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

//%%%%%%%%%%%%
//  Constructor. Takes in configuration file location. If a file does not exist, it will
//      be created!
//%%%%%%%%%%%%
dev::ConfigurationParser::ConfigurationParser(std::string location) : _location(location)
{
    refresh();
}

//%%%%%%%%%%%%
//  Reads the configuration file into the map
//%%%%%%%%%%%%
void dev::ConfigurationParser::refresh()
{
    //Erase the contents of _cfdata
    _cfdata.clear();

    //Reads the configuration file
    std::ifstream fin(_location.c_str(), std::ios::in);

    //If the file does not exist, create a new file!
    if(!fin.is_open()) std::ofstream(_location.c_str(), std::ios::out).close();

    std::string buffer;
    //Extract each line!
    while(std::getline(fin, buffer, '\n'))
    {
        //Check if the index of the hash does exist! If it exists, get it out of the way!
        if(dev::charPos(buffer, '#') != -1) buffer = buffer.substr(0, dev::charPos(buffer, '#') - 1);
        dev::trim(buffer);
        //Check to make sure that it isn't a line with only comments!
        if(buffer.size() != 0)
        {
            std::string key, value;
            if(dev::charPos(buffer, '=') == -1) key = buffer;
            else
            {
                size_t eqloc = dev::charPos(buffer, '=');
                key = buffer.substr(0, eqloc);
                value = buffer.substr(eqloc + 1, buffer.size());
            }
            dev::trim(key);
            dev::trim(value);
            _cfdata[key] = value;
        }
    }

    fin.close();
}

void dev::ConfigurationParser::flush()
{
    std::ofstream fout(_location.c_str(), std::ios::out);
    if(fout.is_open())
    {
        for(std::map<std::string, std::string>::const_iterator it = _cfdata.begin(); it != _cfdata.end(); ++it)
        {
            fout << it->first << "=" << it->second << std::endl;
        }
    }

    fout.close();
}

std::string dev::ConfigurationParser::get(std::string key) { return _cfdata[key]; }
void dev::ConfigurationParser::add(std::string key, std::string value) { _cfdata[key] = value; }
void dev::ConfigurationParser::remove(std::string key) { _cfdata.erase(key); }
std::string& dev::ConfigurationParser::operator[](std::string key) { return _cfdata[key]; }

bool dev::ConfigurationParser::tryBool(std::string key)
{
    std::string b = dev::itrim(dev::tolower(_cfdata[key]));
    return b == "yes" || b == "1" || b == "true" || b == "on";
}
