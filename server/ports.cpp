#include "ports.hpp"
#include "../devlib/configuration.hpp"
#include "../devlib/string.hpp"
#include "../buildconf.h"
#include "../devlib/log.hpp"
int port_connector;
int port_interface;
#ifndef DEFAULT_PORT_CONNECTOR
#define DEFAULT_PORT_CONNECTOR 5801
#endif
#ifndef DEFAULT_PORT_INTERFACE
#define DEFAULT_PORT_INTERFACE 5800
#endif
void loadPorts()
{
    dev::getLogger().put("Scanning configuration for port configuration data");
    if(dev::getCFG()["Port_Connector"].size() == 0)
    {
        dev::getLogger().put("Set the connector port to " + dev::toString(DEFAULT_PORT_CONNECTOR));
        dev::getCFG()["Port_Connector"] = dev::toString(DEFAULT_PORT_CONNECTOR);
    }
    if(dev::getCFG()["Port_Interface"].size() == 0)
    {
        dev::getLogger().put("Set the interface port to " + dev::toString(DEFAULT_PORT_INTERFACE));
        dev::getCFG()["Port_Interface"] = dev::toString(DEFAULT_PORT_INTERFACE);
    }
    dev::getCFG().flush();
    dev::fromString<int>(port_connector, dev::getCFG()["Port_Connector"]);
    dev::fromString<int>(port_interface, dev::getCFG()["Port_Interface"]);
}
