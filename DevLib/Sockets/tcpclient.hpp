#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP
#include "../Exceptions.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netdb.h>
namespace dev
{
    class TcpClient
    {
    private:
        int fd;
        struct sockaddr_in server_address;
        struct hostent* server;
        void start(std::string address, int port);
    public:
        TcpClient(std::string location);
        TcpClient(std::string location, int port);
        ~TcpClient();
        std::string get(int length);
        void put(std::string data);
    };
}
#endif // TCPCLIENT_HPP
