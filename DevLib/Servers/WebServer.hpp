#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "../Sockets/tcpserver.hpp"
#include "../compat.hpp"
#ifdef CXX11_SUPPORTED
#include <unordered_map>
#include <functional>
#else
#include <map>
#endif
#include <string>
namespace dev
{
//I'd rather use an unordered map here. However, it only is supported in C++11 and above! Not really required, however, as c++11 is required
//to compile the rest of this!
#ifdef CXX11_SUPPORTED
    typedef std::unordered_map<std::string, std::string> strmap;
#else
    typedef std::map<std::string, std::string> strmap;
#endif

#define WebServerGet true;
#define WebServerPost false;
#ifndef HTTP_LINE_ENDING
#define HTTP_LINE_ENDING "\r\n"
#endif

    class HttpServerRequest
    {
    public:
        dev::strmap queries;
        dev::strmap _POST;
        dev::strmap _GET;
        dev::strmap headers;
        dev::strmap incoming_headers;

        std::string request;
        std::string response;
        std::string foreign_ip;
        std::string path;
        std::string http_ver;
        std::string mime;
        std::string charset;

        dev::TcpSocketServerConnection connection;

        bool request_type;  //True if GET, false of POST

        int status_code;
        int foreign_port;

        void setResponse(std::string data) { response = data; }
    };

    class HttpServer : public dev::TcpSocketServer
    {
    private:
        virtual void request_handler(dev::HttpServerRequest& request);
        void worker(dev::TcpSocketServerConnection connection);
        dev::HttpServerRequest parseRequest(dev::TcpSocketServerConnection& request);
    };
#ifdef CXX11_SUPPORTED
    class HttpServerInALambda : public dev::HttpServer
    {
    private:
        std::function<void(dev::HttpServerRequest& request)> request_handler_func;
        void request_handler(dev::HttpServerRequest& request)
        {
            try
            {
                request_handler_func(request);
            }
            catch(std::exception& e)
            {
                std::cout << "Request Handler Terminated Due to Thrown Exception: " << e.what() << std::endl;
            }
        }
    public:
        HttpServerInALambda(std::function<void(dev::HttpServerRequest& request)> func) : request_handler_func(func) {}
    };
#endif
}
#endif // WEBSERVER_H
