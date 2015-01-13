#ifndef MISC_HPP
#define MISC_HPP
#include <string>
#include <vector>
namespace dev
{
    std::string getTime();
    template<typename type> void addVectors(std::vector<type>& main, std::vector<type> secondary)
    {
        for(unsigned int i = 0; i < secondary.size(); i++)
        {
            main.push_back(secondary[i]);
        }
    }
}
#endif // MISC_HPP
