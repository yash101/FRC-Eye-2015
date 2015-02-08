#ifndef CORE_HPP
#define CORE_HPP
#include <exception>
#include <string>
namespace dev
{
    class exception : public std::exception
    {
    private:
        std::string _message;
        int _code;
    public:
        exception(std::string message, int code) : _message(message), _code(code) {}
        exception(std::string message) : _message(message), _code(0) {}
        exception(int code) : _message("exeption thrown!"), _code(code) {}
        ~exception() throw() {}
        virtual const char* what() const throw() { return _message.c_str(); }
        int getCode() { return _code; }
    };
}
#endif // CORE_HPP
