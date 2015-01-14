#ifndef VISIONSERVER_HPP
#define VISIONSERVER_HPP
#include "../DevLib/Sockets/tcpserver.hpp"
#include <mutex>
#include <thread>
#include <opencv2/highgui/highgui.hpp>
#include "thresholdserver.hpp"
namespace VisionServer
{
    enum CaptureMode
    {
        CAP_DEVICE = 0,
        CAP_LOCATION = 1
    };

    class VisionServer : public dev::TcpSocketServer
    {
    private:
        ::VisionServer::ThresholdServer ThresholdHost;
        void initializeThresholdHost();
    protected:
        void worker(dev::TcpSocketServerConnection connection);
        void find(dev::TcpSocketServerConnection& connection);
    public:
        VisionServer(int MainPort, int ThresholdConfigurationPort);
    };
}
#endif // VISIONSERVER_HPP
