#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP
#include "../compat.hpp"

#ifdef POSIX
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <mutex>

#include "../Exceptions.hpp"

#include <functional>

#define DEFAULT_TCP_ARRAY_LENGTH    1024    //bytes
namespace dev
{
    class TcpSocketServerConnection
    {
    public:
        //Elements to access!
        int connid;
        int c;

        //Constructors
        TcpSocketServerConnection() {}
        ~TcpSocketServerConnection() { close(connid); }

        //Functions
        void put(char byte);
        void put(std::string str);
        int read();
        std::string read(int length);
        std::string getline(char end);
        std::string getline(std::string end);
        void close_socket() { close(connid); }
    };

    //The Socket Server Class
    class TcpSocketServer
    {
    private:
        int fd;
        struct sockaddr_in address;
        void listener();
        virtual void worker(dev::TcpSocketServerConnection connection);
        void vhost(int connectionid);
    public:
        TcpSocketServer();
        void start(int port);
        void shutdown() { close(fd); }
    };

    //A Lambda-based TCP Socket Server Class (:D)
    class TcpSocketServerInALambda : public dev::TcpSocketServer
    {
    private:
        std::function<void(dev::TcpSocketServerConnection connection)> func;
    public:
        TcpSocketServerInALambda(std::function<void(dev::TcpSocketServerConnection connection)> worker_func)
        { func = worker_func; }
        void worker(dev::TcpSocketServerConnection connection) { func(connection); }
    };

    class hello_world_tcp_socket_server : public dev::TcpSocketServer
    {
        void worker(dev::TcpSocketServerConnection connection)
        {
            connection.put("Welcome to DevLib! It seems as though you are enjoying this library's TCP server\n");
            connection.put("This example is acually quite simple!\n");
            connection.put("Hop down to the bottom of DevLib/Sockets/tcpserver.hpp and find this class to learn more!\n");
            connection.put("Now, what is your name? ");
            std::string x = connection.read(1024);
            connection.put("It was my pleasure talking to you, " + x + "!\n");
        }
    };
}

#endif // TCPSOCKET_HPP
