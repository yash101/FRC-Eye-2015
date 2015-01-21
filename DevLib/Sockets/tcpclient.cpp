#include "tcpclient.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include "../string.hpp"
#include <iostream>
dev::TcpClient::TcpClient(std::string location)
{
    if(dev::charPos(location, ':') == -1)
    {
        throw dev::SocketException("DEVLIB ERROR. Unable to parse address and port");
    }
    std::string address = location.substr(0, dev::charPos(location, ':'));
    int port = std::atoi(location.substr(dev::charPos(location, ':') + 1, location.size()).c_str());
    start(address, port);
}

void dev::TcpClient::start(std::string address, int port)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        throw dev::SocketException("DEVLIB ERROR. Unable to create socket");
    }

    server = gethostbyname(address.c_str());
    if(server == NULL)
    {
        throw dev::SocketException("DEVLIB ERROR. Unable to resolve hostname");
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);
    if(connect(fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
    {
        throw dev::SocketException("DEVLIB ERROR. Unable to connect to socket server");
    }
}

std::string dev::TcpClient::get(int length)
{
    char* buffer = new char[length];
    if(read(fd, buffer, length) < 0)
    {
        throw dev::SocketException("DEVLIB ERROR. Read from socket failed");
    }
    std::string str = dev::toString(buffer);
    delete[] buffer;
    return str;
}

void dev::TcpClient::put(std::string data)
{
    if(write(fd, data.c_str(), data.size()) < 0)
    {
        throw dev::SocketException("DEVLIB ERROR. Write to socket failed");
    }
}

dev::TcpClient::~TcpClient()
{
    close(fd);
}
