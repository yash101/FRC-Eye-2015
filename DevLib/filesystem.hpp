#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <fstream>
#include <string>
namespace dev
{
    bool WriteFile(std::string filename, std::string data);
    void WriteFileAsync(std::string filename, std::string data);
    std::string ReadFile(std::string filename);
    bool StatFile(std::string filename);
    void DeleteFile(std::string filename);
    void CreateDirectory(std::string filename);
    void CreateDirectoryTree(std::string filename);
}
#endif // FILESYSTEM_H
