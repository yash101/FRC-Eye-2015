#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "../dlib/server.h"
class Interface : public dlib::server_http
{
private:
    std::string _savefile;
public:
    Interface(std::string savefile) : _savefile(savefile) {}
    const std::string on_request(const dlib::incoming_things& incoming, dlib::outgoing_things& outgoing);
};
#endif // INTERFACE_HPP
