#ifndef WEBGUI_HPP
#define WEBGUI_HPP
#include <unordered_map>
#include "../Servers/http.hpp"
namespace dev
{
    class widget
    {
    };

    class webgui : public dev::http_server
    {
    private:
        std::unordered_map<std::string, dev::widget> _widgets;
    public:
        webgui(int port) { start(port); }
        void addWidget(std::string name, dev::widget wid);
        dev::widget operator[](std::string name) { return _widgets[name]; }
        void removeWidget(std::string name);
    };
}

#endif // WEBGUI_HPP
