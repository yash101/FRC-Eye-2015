#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP
#include "../dlib/server.h"
class Connector : public dlib::server_iostream
{
private:
    void on_connect(
        std::istream& in,
        std::ostream& out,
        const std::string& foreign_ip,
        const std::string& local_ip,
        unsigned short foreign_port,
        unsigned short local_port,
        dlib::uint64 connection_id
    );
};
#endif // CONNECTOR_HPP
