#include "../filesystem.hpp"
#include "../string.hpp"
#include <thread>

bool dev::WriteFile(std::string filename, std::string data)
{
    std::ofstream fout(filename.c_str());
    if(fout.is_open())
    {
        fout << data;
        fout.close();
        return true;
    }
    return false;
}

void dev::WriteFileAsync(std::string filename, std::string data)
{
    std::thread([&](){dev::WriteFile(filename, data);});
}

std::string dev::ReadFile(std::string filename)
{
    std::string out;
    std::ifstream fin(filename.c_str());
    if(fin.is_open())
    {
        out = dev::toString(fin.rdbuf());
        fin.close();
        return out;
    }
    return "Unable to open file!";
}

bool dev::StatFile(std::string filename)
{return false;}

void CreateDirectory(std::string filename)
{/*NI*/}

void CreateDirectoryTree(std::string filename)
{/*NI*/}
