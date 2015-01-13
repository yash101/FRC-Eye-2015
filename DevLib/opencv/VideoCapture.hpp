#ifndef VIDEOCAPTURE_HPP
#define VIDEOCAPTURE_HPP
#include <string>
#include <thread>
#include <mutex>
namespace dev
{
    class VideoCapture
    {
    private:
    public:
        VideoCapture(int captureDevice);
        VideoCapture(std::string captureLocation);
    };
}
#endif // VIDEOCAPTURE_HPP
