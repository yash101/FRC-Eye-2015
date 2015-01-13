#include "visionserver.hpp"
#include "../DevLib/string.hpp"
#include "visionalgs.hpp"
#include "../DevLib/Exceptions.hpp"
#include <opencv2/imgproc/imgproc.hpp>

//----------------------------------------------------------//
//  Constructor for camera location based off string
VisionServer::VisionServer::VisionServer(int port, std::string location, int ThreshPort) : ThresholdHost(ThreshPort)
{
    start(port);
    cam = cv::VideoCapture(location);
    if(!cam.isOpened()) { throw dev::exception("Unable to open the image!"); }
    initializeThresholdHost();
    std::thread(&VisionServer::VisionServer::cameraThread, this).detach();
}

//----------------------------------------------------------//
//  Constructor for camera based off a camera device ID
VisionServer::VisionServer::VisionServer(int port, int capDev, int ThreshPort) : ThresholdHost(ThreshPort)
{
    start(port);
    cam = cv::VideoCapture(capDev);
    if(!cam.isOpened()) { throw dev::exception("Unable to open the image!"); }
    initializeThresholdHost();
    std::thread(&VisionServer::VisionServer::cameraThread, this).detach();
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
            cmd = connection.getline(' ');
        }
        catch(std::exception& e)
        {
        }

        if(cmd == "FIND")
        {
            find(connection);
        }
    }
}

//----------------------------------------------------------//
//  This function selects the appropriate algorithm, runs it and returns the results
void VisionServer::VisionServer::find(dev::TcpSocketServerConnection& connection)
{
    std::string whatToFind = connection.getline(' ');
    if(whatToFind == "YELLOWTOTE")
    {
        std::string data = ::VisionServer::toString(::VisionServer::findYellowTotes(getImage(), ThresholdHost.getHigh("YellowTote"), ThresholdHost.getLow("YellowTote")));
        connection.put("CLEN " + dev::toString(data.size()) + " " + data);
    }
//    else if(whatToFind == "GREYTOTE")
//    {
//        std::string data = ::VisionServer::toString(::VisionServer::findYellowTotes(getImage()), ThresholdHost.getHigh("GreyTote"), ThresholdHost.getLow("YellowTote"));
//        connection.put("CLEN " + dev::toString(data.size()) + " " + data);
//    }
//    else if(whatToFind == "LITTER")
//    {
//        std::string data = ::VisionServer::toString(::VisionServer::findYellowTotes(getImage()), ThresholdHost.getHigh("Litter"), ThresholdHost.getLow("YellowTote"));
//        connection.put("CLEN " + dev::toString(data.size()) + " " + data);
//    }
//    else if(whatToFind == "GARBAGECAN")
//    {
//        std::string data = ::VisionServer::toString(::VisionServer::findYellowTotes(getImage()), ThresholdHost.getHigh("GarbageCan"), ThresholdHost.getLow("YellowTote"));
//        connection.put("CLEN " + dev::toString(data.size()) + " " + data);
//    }
    connection.put(' ');
}

//----------------------------------------------------------//
//  This function runs the camera. The camera is run in a separate thread to prevent
//      the camera buffer from overflowing and thus inducing terrible lag
void VisionServer::VisionServer::cameraThread()
{
    //A temporary matrix. This will stay in scope throughout so we can prevent
    //constantly allocating and deallocating.
    cv::Mat tmp;
    //Check if we were able to open the camera and begin to start retrieve the image!
    if(cam.isOpened())
    {
        while(true)
        {
            cam >> tmp;
            if(!tmp.empty())
            {
                cam_thread_lock.lock();
                ThreadedImage = tmp.clone();
                cam_thread_lock.unlock();
            }
        }
    }
}

//----------------------------------------------------------//
//  Retrieves an image from the camera
cv::Mat VisionServer::VisionServer::getImage()
{
    cv::Mat x;
    cam_thread_lock.lock();
    x = ThreadedImage.clone();
    cam_thread_lock.unlock();
    return x;
}

void VisionServer::VisionServer::initializeThresholdHost()
{
    ThresholdHost.add("YellowTote", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
    ThresholdHost.add("GreyTote", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
    ThresholdHost.add("Litter", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
    ThresholdHost.add("GarbageCan", cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), ::VisionServer::ThresholdExtras(1, 1));
}
