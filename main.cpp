#define CALCULATE_PI_DYNAMIC
#include "src/visionserver.hpp"
#include <signal.h>
#include "src/camera.hpp"
#include "src/valueserver2.hpp"

void x(int x)
{
    exit(EXIT_SUCCESS);
}

int main()
{
    //for debugging. hopefully this runs the destructor of the server when quitting
    signal(SIGINT, x);
    signal(SIGTERM, x);
    signal(SIGSEGV, x);
    signal(SIGCHLD, x);

    VisionServer::VisionServer vis(1234, 12345);
    ValueServer2 val("HelloWorld.dat", 2345);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1024));
    }
}
