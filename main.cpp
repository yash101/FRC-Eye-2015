#define CALCULATE_PI_DYNAMIC
#include "src/visionserver.hpp"
#include "src/log.hpp"
#include <signal.h>
#include "DevLib/math.hpp"
#include "DevLib/Sockets/tcpclient.hpp"
#include <iostream>
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include "src/camera.hpp"

void x(int x)
{
    exit(EXIT_SUCCESS);
}

std::string getline(dev::TcpClient& client, char end)
{
    std::string x;
    while(true)
    {
        x += client.get(1);
        if(x[x.size() - 1] == end) return x.substr(0, x.size() - 1);
    }
}

int main()
{
    ::camera::Cam0::stop();
    //for debugging. hopefully this runs the destructor of the server when quitting
    signal(SIGINT, x);
    signal(SIGTERM, x);
    signal(SIGSEGV, x);
    signal(SIGCHLD, x);

    std::cout << "SIN THETA[PI/2]=[" << std::setprecision(200) << dev::sin<long double>(dev::pi / 2.0) << "]" << std::endl;
    std::cout << "SIN THETA[PI/2]=[" << std::setprecision(200) << dev::cos<long double>(dev::pi / 2.0) << "]" << std::endl;
    std::cout << "SIN THETA[PI/2]=[" << std::setprecision(200) << dev::tan<long double>(dev::pi / 2.0) << "]" << std::endl;
    std::cout << "SIN THETA[PI/2]=[" << std::setprecision(200) << std::sin(dev::pi / 2.0) << "]" << std::endl;

//    VisionServer::VisionServer vis(1234, 12345);

//    while(true)
//    {
//        std::this_thread::sleep_for(std::chrono::seconds(1024));
//    }
}
