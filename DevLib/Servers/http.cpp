#include "http.hpp"
#include <cstring>
#include "../string.hpp"
#include <vector>
#define HEND HTTP_LINE_ENDING

//Default constructor for the http server
dev::http_server::http_server() {}
void dev::http_server::worker(dev::TcpSocketServerConnection connection)
{
    http_session session;
    try
    {
        session.connection = connection;
        session.parse_request();
        session.prepare_request();
        request_handler(session);
        send_response(session);
    }
    catch(std::exception& e)
    {
        std::cout << "Error while processing request! what() -> " << e.what() << std::endl;
        session.connection.close_socket();
        return;
    }
}

void dev::http_session::prepare_request()
{
    headers["content-type"] = "text/html";
    headers["server"] = "DevLibWeb/2.0";
    status_code = 200;
}

void dev::http_session::parse_request()
{
    //Determine the request type.
    std::string method = connection.getline(' ');
    if(method == "GET") { request_type = GET; }
    else if(method == "POST") { request_type = POST; }
    else if(method == "HEAD") { request_type = HEAD; }
    else { throw dev::HTTPException("Unknown HTTP Method!"); }

    //Determine the path the client is requesting
    path = dev::itrim(connection.getline(' '));

    //Determine the protocol the client wants us to use
    protocol = dev::itrim(connection.getline("\r\n"));

    //Download the rest of the request header
    std::string buffer;
    while(true)
    {
        char x = connection.read(1)[0];
        buffer += x;
        if(buffer.size() >= 4)
        {
            if(buffer.substr(buffer.size() - 4, buffer.size()) == "\r\n\r\n")
            {
                break;
            }
        }
    }

    std::stringstream in(buffer);
    //Get the lines out of the request
    while(std::getline(in, buffer, '\n'))
    {
        //Process each line and add it in!
        if(dev::charPos(buffer, ':') != -1)
        {
            std::string key = dev::tolower(buffer.substr(0, dev::charPos(buffer, ':')));
            std::string value = buffer.substr(dev::charPos(buffer, ':') + 1, buffer.size() - 1);
            if(std::isspace(value[0])) { value = value.substr(1, value.size()); }
            incoming_headers[key] = value;
        }
    }

    if(dev::charPos(path, '?') != -1)       //Ensure that there is an actual query to process
    {
        std::string query = path.substr(dev::charPos(path, '?') + 1, path.size());
        path = path.substr(0, dev::charPos(path, '?'));

        std::stringstream x(query);
        while(std::getline(x, buffer, '&')) //Explode the query into it's different parts!
        {
            std::stringstream z(buffer);
            std::string key, value;
            std::getline(z, key, '=');
            std::getline(z, value);
            if(key != "")
            {
                get[key] = dev::urldecode(value);
                queries[key] = dev::urldecode(value);
            }
        }
    }

    //Check if we recieved any data in the POST (get the joke there?)
    if(request_type == POST)
    {
        //Basic POST requests
        if(dev::tolower(incoming_headers["content-type"]).find("form-data") == std::string::npos)
        {
            int length = std::atoi(incoming_headers["content-length"].c_str());
            std::string postvars;
            //Download the POST data
            for(int i = 0; i < length; i++)
            {
                postvars += (char) connection.read();
            }

            std::stringstream x(postvars);
            while(std::getline(x, buffer, '&')) //Explode the query into it's different parts!
            {
                std::stringstream z(buffer);
                std::string key, value;
                std::getline(z, key, '=');
                std::getline(z, value);
                if(key != "")
                {
                    post[key] = dev::urldecode(value);
                    queries[key] = dev::urldecode(value);
                }
            }
        }
        else
        {
            std::string boundary = incoming_headers["content-type"].substr(dev::charPos(incoming_headers["content-type"], '=') + 1, incoming_headers["content-type"].size() - 1);
            int length = std::atoi(incoming_headers["content-length"].c_str());
            std::string inc;
            //Download the request body, which contains the uploaded files
            for(int i = 0; i < length; i++)
            {
                inc += (char) connection.read();
            }
            if(inc[inc.size()] == '-' || inc[inc.size() - 1] == '-') { inc = inc.substr(0, inc.size() - 2); }

            //The general purpose buffer that we will use
            std::string buffer;
            //This contains the entire request body, for us to break up and process!
            std::stringstream x(inc);
            std::vector<std::string> lines;
            while(dev::getline(buffer, boundary, x))
            {
                lines.push_back(buffer);
            }

            //Clear our buffer so that we don't start appending to it accidentally
            buffer = "";
            //Process all of the boundaries
            if(lines.size() > 1)
            {
                for(unsigned int i = 1; i < lines.size(); i++)
                {
                    //The current request part
                    std::stringstream y(lines[i]);                      //Stringstreams allow us to serialize data and then process them
                                                                        //We load this part of the body into it so we can process it!
                    
                    dev::getline(buffer, "\r\n", y);                    //Get the first line (content-disposition)
                    
                    std::string name = buffer.substr(dev::charPos(buffer, ';') + 8, buffer.size() - 1);     //Get the name of the field
                    name = name.substr(0, dev::charPos(name, '"'));     //Process the name and get rid of the quotation marks!

                    if(buffer.find("filename") != std::string::npos)    //If it is just a file value (filename field found)
                    {
                        dev::getline(buffer, "\r\n\r\n", y);            //Get the mime of the file

                        //**get the rest of the file!
                        buffer = "";                                    //Reset our buffer string because we are about to write to it!
                        while(y.peek() != EOF) { buffer += (char) y.get(); }    //Read the stringstream till it's end!
                        buffer = buffer.substr(0, buffer.size() - 4);
                        files[name] = buffer;
                    }
                    else
                    {
                        std::stringstream out;
                        out << y.rdbuf();
                        std::string x = out.str().substr(0, out.str().size() - 2);
                        queries[name] = x;
                        post[name] = x;
                    }
                }
            }
        }
    }
}

//Sends the response back to the client so it can enjoy!
void dev::http_server::send_response(dev::http_session& session)
{
    //Send first line. The response shall be send now!
    session.connection.put("HTTP/1.0 " + dev::toString(session.status_code) + " " + dev::http_status(session.status_code) + HEND);
    for(std::unordered_map<std::string, std::string>::const_iterator it = session.headers.begin(); it != session.headers.end(); ++it)
    {
        session.connection.put(it->first + ": " + it->second + HEND);
    }
    session.connection.put("content-length: " + dev::toString(session.response.size()) + HEND);
    session.connection.put(HEND);
    session.connection.put(session.response);
    session.connection.close_socket();
}

//To be overwritten
void dev::http_server::request_handler(dev::http_session& session)
{
    session.response = "This web server is not for direct use!";
}
