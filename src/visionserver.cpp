#include "visionserver.hpp"
#include "../DevLib/string.hpp"
#include "visionalgs.hpp"
#include "../DevLib/Exceptions.hpp"
#include "camera.hpp"
#include <opencv2/imgproc/imgproc.hpp>

//----------------------------------------------------------//
//  Constructor for camera based off a camera device ID
VisionServer::VisionServer::VisionServer(int MainPort, int ThresholdConfigurationPort) : ThresholdHost(ThresholdConfigurationPort)
{
    start(MainPort);
    initializeThresholdHost();
}

//----------------------------------------------------------//
//  Socket server listener. This awaits connections to start playing with
void VisionServer::VisionServer::worker(dev::TcpSocketServerConnection connection)
{
    std::string cmd;
    while(true)
    {
        try
        {
            cmd = connection.getline('\n');
            std::cout << cmd << std::endl;
        }
        catch(std::exception& e)
        {
        }

        if(cmd.find("FIND") != std::string::npos)
        {
            find(connection);
        }
    }
}

//----------------------------------------------------------//
//  This function selects the appropriate algorithm, runs it and returns the results
void VisionServer::VisionServer::find(dev::TcpSocketServerConnection& connection)
{
    connection.put("Reached Here!\n");
    std::string whatToFind = connection.getline('\n');
    std::cout << whatToFind << std::endl;
    whatToFind = "YELLOWTOTE";
    std::string data = "";
    if(whatToFind.find("YELLOWTOTE") != std::string::npos)
    {
        connection.put("Processing Yellow Totes!\n");
        data = ::VisionServer::toString(::VisionServer::findPolygons("YellowTote", ThresholdHost));
    }
    else if(whatToFind.find("GREYTOTE") != std::string::npos)
    {
        data = ::VisionServer::toString(::VisionServer::findPolygons("GreyTote", ThresholdHost));
    }
    else if(whatToFind.find("LITTER") != std::string::npos)
    {
        data = ::VisionServer::toString(::VisionServer::findPolygons("Litter", ThresholdHost));
    }
    else if(whatToFind.find("GARBAGECAN") != std::string::npos)
    {
        data = ::VisionServer::toString(::VisionServer::findPolygons("GarbageCan", ThresholdHost));
    }
    connection.put("CLEN " + dev::toString(data.size()) + " " + data + " ");
}

void VisionServer::VisionServer::initializeThresholdHost()
{
    ThresholdHost.add("YellowTote", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
    ThresholdHost.add("GreyTote", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
    ThresholdHost.add("Litter", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
    ThresholdHost.add("GarbageCan", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
}
