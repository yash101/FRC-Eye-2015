#include "udpserver.hpp"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "../string.hpp"

dev::UdpSocketServer::UdpSocketServer() {}
void dev::UdpSocketServer::start(int port)
{
    //Create our socket
    int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketFD < 0) { throw dev::SocketException("Unable to create new socket!"); }
    memset((char*) &server_address, 0, sizeof(&server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = port;

    int b = bind(socketFD, (struct sockaddr*) &server_address, sizeof(server_address));
    if(b < 0) { throw dev::SocketException("Unable to bind to socket at port " + dev::toString(port)); }
}

void dev::UdpSocketServerConnection::put(std::string str)
{

}

void dev::UdpSocketServer::worker(dev::UdpSocketServerConnection connection) {}
