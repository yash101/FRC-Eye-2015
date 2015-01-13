#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <exception>
#include <string>
namespace dev
{
    class SocketException : public std::exception
    {
    private:
        std::string _message;
    public:
        ~SocketException() throw() {}
        SocketException(std::string message) : _message(message) {}
        virtual const char* what() const throw() { return _message.c_str(); }
    };

    class AIException : public std::exception
    {
    private:
        std::string _message;
    public:
        ~AIException() throw() {}
        AIException(std::string message) : _message(message) {}
        virtual const char* what() const throw() { return _message.c_str(); }
    };

    class HTTPException : public std::exception
    {
    private:
        std::string _message;
    public:
        ~HTTPException() throw() {}
        HTTPException(std::string message) : _message(message) {}
        virtual const char* what() const throw() { return _message.c_str(); }
    };

    class exception : public std::exception
    {
    private:
        std::string _message;
    public:
        ~exception() throw() {}
        exception(std::string message) : _message(message) {}
        virtual const char* what() const throw() { return _message.c_str(); }
    };

    typedef dev::exception VectorException;
}
#endif // EXCEPTIONS_HPP
