#include "utilities/camera.hpp"
#include "devlib/configuration.hpp"
#include "server/fork.hpp"
#include "server/ports.hpp"
int main()
{
    boot::fork();
    loadPorts();
}
