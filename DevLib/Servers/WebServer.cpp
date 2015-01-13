#include "WebServer.hpp"
#include "../string.hpp"
#include "../misc.hpp"

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <iostream>

#ifndef HTTP_SERVER_MAX_REQUEST_SIZE
#define HTTP_SERVER_MAX_REQUEST_SIZE (long long) 8 * 1024 * 1024 * 256 //Max request size is 256MiB!
#endif

void parseGET(dev::HttpServerRequest& req)
{
    std::string buffer;
    int loc = dev::charPos(req.path, '?');
    if(loc > 0 && req.path.size() > loc)   //Check to see if there actually is a '?' in the path, and make sure it isn't empty either
    {
        std::string queries = req.path.substr(loc + 1, req.path.size());
        std::vector<std::string> fields;
        std::stringstream s(queries);
        while(std::getline(s, buffer, '&')) { fields.push_back(buffer); }
        for(unsigned int i = 0; i < fields.size(); i++)
        {
            std::string x = fields[i];
            std::string key;
            std::string value;
            int pos = dev::charPos(x, '=');
            if(pos == -1)
            {
                key = x;
                value = "";
            }
            else
            {
                key = x.substr(0, pos);
                value = x.substr(pos + 1, x.size());
            }
            req.queries[key] = value;
            req._GET[key] = value;
        }
    }
}

void parsePOST(dev::HttpServerRequest& req)
{
}

void prepare_request(dev::HttpServerRequest& req)
{
    req.response = "";
    req.status_code = 200;
    req.mime = "text/html";
    req.headers["Server"] = "DevLibWeb/1.0";
    req.charset = "UTF-8";
    parseGET(req);
    parsePOST(req);
}

void send_response(dev::HttpServerRequest& req)
{
    std::stringstream response;
    response << req.http_ver << " " << req.status_code << " OK" << HTTP_LINE_ENDING;
    response << "Content-Type: " << req.mime << "; Charset=" << req.charset << HTTP_LINE_ENDING;
    for(dev::strmap::const_iterator it = req.headers.begin(); it != req.headers.end(); ++it)
    {
        response << it->first << ": " << it->second << HTTP_LINE_ENDING;
    }
    response << "Content-Length: " << req.response.size() << HTTP_LINE_ENDING;
    response << HTTP_LINE_ENDING;
    response << req.response;
    req.connection.put(response.str());
}

void dev::HttpServer::worker(dev::TcpSocketServerConnection connection)
{
    try
    {
        dev::HttpServerRequest req = parseRequest(connection);
        if(req.path == "") { std::cout << "Empty path!" << std::endl; return; }
        req.connection = connection;
        prepare_request(req);
        request_handler(req);
        send_response(req);
    }
    catch(dev::SocketException& ex)
    {
        std::cout << "Unable to complete request because of thrown exception: [" << ex.what() << "]" << std::endl;
    }
    return;
}

dev::HttpServerRequest generateRequest(std::vector<std::string> lines, std::string in)
{
    std::string buffer;
    dev::HttpServerRequest request;
    request.request = in;

    //We can't put anything in our request!
    if(lines.size() < 1) { return request; }

    //First Line
    std::stringstream buf1(lines[0]);
    std::vector<std::string> topLineStuff;
    while(std::getline(buf1, buffer, ' ')) { topLineStuff.push_back(buffer); }
    if(topLineStuff.size() < 2) { return request; }
    request.request_type = topLineStuff[0] == "GET";
    request.path = topLineStuff[1];
    request.http_ver = "HTTP/1.0";

    if(lines.size() < 2) { return request; }
    for(unsigned int i = 1; i < lines.size(); i++)
    {
        std::string val;
        int colonpos = dev::charPos(lines[i], ':');
        if(colonpos == -1) { continue; }
        if((unsigned int) colonpos == lines[i].size())
        {
            val = "";
        }
        if((unsigned int) colonpos <= lines[i].size() + 2)
        {
            val = lines[i].substr(colonpos + 2, lines[i].size());
        }
        request.incoming_headers[buffer] = val;
    }

    return request;
}

dev::HttpServerRequest dev::HttpServer::parseRequest(dev::TcpSocketServerConnection& connection)
{
    std::string buffer;
    std::string buffer2;

    //Download the request from the client
    while(buffer2.size() < HTTP_SERVER_MAX_REQUEST_SIZE)
    {
        buffer2 = buffer2 + connection.read(8192);  //Read 8 kilobyte chunks to keep ze speed up!
        //Check to see if we have hit the end of the request before we begin processing it!
        //We check to see if the last 4 characters are \r\n\r\n, the typical ending of a request
        if(buffer2.size() > 4)
        {
            if(buffer2.substr(buffer2.size() - 4, buffer2.size()) == "\r\n\r\n")
            {
                break;
            }
        }
    }

    //Split our request into lines for easier processing!
    std::stringstream str(buffer2);
    std::vector<std::string> lines;
    while(std::getline(str, buffer, '\n'))
    {
        if(buffer != "\r\n" && buffer.size() > 1)
        {
            lines.push_back(buffer.substr(0, buffer.size() - 2));
        }
    }

    //Begin the processing of our request!
    return generateRequest(lines, buffer2);
}

void dev::HttpServer::request_handler(dev::HttpServerRequest& request)
{
    request.response = "Hello World!";
}
