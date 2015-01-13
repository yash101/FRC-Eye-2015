#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP
#include <sys/socket.h>
#include <mutex>
#include <netinet/in.h>
#include "../Exceptions.hpp"

namespace dev
{
    class UdpSocketServerConnection
    {
        int connid;
        int c;

        UdpSocketServerConnection() {}

        void put(char byte);
        void put(std::string str);
        std::string read(int length);
        int read();
    };

    class UdpSocketServer
    {
    private:
        int socketFD;       //The file descriptor for the socket
        struct sockaddr_in server_address;
        void listener();
        std::mutex allow_accept;
        bool accepting;
        virtual void worker(dev::UdpSocketServerConnection connection);
    public:
        UdpSocketServer();
        void start(int port);
        ~UdpSocketServer();
    };
}

#endif // UDPSOCKET_HPP
