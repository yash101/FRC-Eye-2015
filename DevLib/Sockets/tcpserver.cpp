#include "tcpserver.hpp"
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include <string.h>
#include "../string.hpp"

//Constructor
dev::TcpSocketServer::TcpSocketServer() {}

//Sets the listening port and starts the server.
void dev::TcpSocketServer::start(int port)
{
    //Create the socket file descriptor
    fd = socket(AF_INET, SOCK_STREAM, 0);
    //Verify that we were able to create the socket server and throw and exception if we didn't
    if(fd < 0) { throw SocketException("Unable to Create Socket!"); }
    //Clear the address structure.
    memset((char*) &address, 0, sizeof(address));
    //Set up the address and prepare to start serving
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    //Bind to the address/port specified. If this isn't successful, an exception will be thrown
    if(bind(fd, (struct sockaddr*) &address, sizeof(address)) < 0) { throw SocketException("Unable to bind to socket!"); }
    //Begin listening in the socket
    listen(fd, 3);
    //Launch a new thread to listen for us. This thread detaches to allow this function to quite afterwards
    std::thread(&dev::TcpSocketServer::listener, this).detach();
}

//Processes Requests. This is just the default connection handler. You are supposed to create your own.
//This just sends back to the client what to do to get started.
void dev::TcpSocketServer::worker(dev::TcpSocketServerConnection connection)
{
    connection.put("Hello World! You have successfully created a DevLib TcpSocketServer! Now, create a new class that extends off this!\n");
    connection.put("Overwrite the dev::TcpSocketServer::worker(dev::TcpSocketServerConnection) and add your code.\n");
    connection.put("dev::TcpSocketServerConnection is the class which allows you to interact with the socket connection!\n");
}

void dev::TcpSocketServer::vhost(int connectionid)
{
    try
    {
        dev::TcpSocketServerConnection connection;
        connection.c = sizeof(struct sockaddr_in);
        connection.connid = connectionid;
        worker(connection);
    }
    catch(std::exception& e)
    {
        std::cout << "Exited worker function due to error: " << e.what() << std::endl;
    }
}

void dev::TcpSocketServer::listener()
{
    while(true)
    {
        int c = sizeof(struct sockaddr_in);
        int newfd = accept(fd, (struct sockaddr*) &address, (socklen_t*) &c);
        if(newfd < 0)
        {
            std::cout << "Failed accepting new connection! Errno set to: " << strerror(errno) << std::endl;
            continue;
        }
        std::thread(&dev::TcpSocketServer::vhost, this, newfd).detach();
    }
}

//Send a character to the client
void dev::TcpSocketServerConnection::put(std::string str)
{
    if(write(connid, str.c_str(), str.size()) < 0)
    {
        throw dev::SocketException("DevLib Error TCP_PUT 0. Unable to write to socket!");
    }
}

//Send a character to the client
void dev::TcpSocketServerConnection::put(char byte)
{
    put(dev::toString(byte));
}

//Reads a std::string from the client!
std::string dev::TcpSocketServerConnection::read(int length)
{
    char* x = new char[length];         //Allocate some memory to read into
    //Attempt to read from the socket. Throw and exception otherwise
    if(recv(connid, x, length, 0) < 0) { throw dev::SocketException("Unable to read from socket!"); }
    //Convert what we recieved to an std::string so we have automatic GC.
    std::string z = dev::toString(x);
    //Deallocate the memory that we previously allocated
    delete[] x;
    //Return the string!
    return z;
}

//This reads a char from the client. Note that we just used the string read command, but only read a single byte
//Note that you will need to cast to a char because we are returning int for platform compatibility
int dev::TcpSocketServerConnection::read()
{
    return (int) read(1)[0];
}

//Downloads a line from the client, ending with the specified character!
std::string dev::TcpSocketServerConnection::getline(char end)
{
    std::string out;
    while(true)
    {
        char c = read(1)[0];
        if(c == end) { break; }
        out += c;
    }
    return out;
}

//Same as below, but ending with a std::string!
std::string dev::TcpSocketServerConnection::getline(std::string end)
{
    std::string out;
    while(true)
    {
        if(out.size() >= end.size())
        {
            if(out.substr(out.size() - end.size(), out.size()) == end)
            {
                return out.substr(0, out.size() - end.size());
            }
        }
        out += (char) read();
    }
}
