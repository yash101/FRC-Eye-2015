#include "src/visionserver.hpp"
#include "src/log.hpp"
#include <signal.h>
#include "DevLib/math.hpp"
#include <iostream>
#include <iomanip>

void x(int x)
{
    exit(EXIT_SUCCESS);
}

int main()
{
    signal(SIGINT, x);
    signal(SIGTERM, x);
    signal(SIGSEGV, x);
    signal(SIGCHLD, x);

    VisionServer::VisionServer vis(1234, 12345);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1024));
    }
}
