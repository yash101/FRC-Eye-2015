#include "interface.hpp"
#include "../devlib/string.hpp"
const std::string Interface::on_request(const dlib::incoming_things& incoming, dlib::outgoing_things& outgoing)
{
    if(incoming.path == "/")
    {
    }
    outgoing.http_return = 404;
    return "<html><body><h1>ERROR 404: Invalid Location!</h1></body></html>";
}
