#ifndef HTTP_HPP
#define HTTP_HPP
#include "../Sockets/tcpserver.hpp"
#include "http_functions.hpp"
#include <string>
#include <unordered_map>
#include <functional>

#ifndef HTTP_MAX_REQUEST_SIZE
#define HTTP_MAX_REQUEST_SIZE (long long) 1024 * 1024 * 32
#endif
#ifndef HTTP_LINE_ENDING
#define HTTP_LINE_ENDING "\r\n"
#endif
namespace dev
{
    enum request_t
    {
        GET = 0,
        POST = 1,
        HEAD = 2
    };

    class http_session
    {
    public:
        dev::request_t request_type;
        //Holds the response to send back
        std::string response;
        //Holds queries from the server!
        std::unordered_map<std::string, std::string> queries;
        std::unordered_map<std::string, std::string> post;
        std::unordered_map<std::string, std::string> get;
        //Holds the incoming files from the server
        std::unordered_map<std::string, std::string> files;
        //Incoming headers
        std::unordered_map<std::string, std::string> incoming_headers;
        std::unordered_map<std::string, std::string> headers;
        //Holds the path the client wants
        std::string path;
        //The requested protocol. Sadly, we only take 1.0!
        std::string protocol;
        //The status code to return.
        short status_code;
        //Holds the socket connection class
        dev::TcpSocketServerConnection connection;

        //Parses the response
        void parse_request();
        //Prepare to send the request (before user code)
        void prepare_request();
    };

    class http_server : public dev::TcpSocketServer
    {
    private:
        void worker(dev::TcpSocketServerConnection connection);
        virtual void request_handler(dev::http_session& session);
        void send_response(dev::http_session& session);
    public:
        http_server();
    };

    class lambda_http_server : public dev::http_server
    {
    private:
        std::function<void(dev::http_session& session)> wfunc;
    public:
        void request_handler(dev::http_session& session)
        {
            wfunc(session);
        }

        lambda_http_server(std::function<void(dev::http_session& session)> func)
        {
            wfunc = func;
        }

        void setWorker(std::function<void(dev::http_session& session)> func)
        {
            wfunc = func;
        }
    };
}
#endif // HTTP_HPP
