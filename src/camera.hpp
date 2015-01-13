#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <mutex>
#include "platform_configuration.hpp"
namespace camera
{
    enum CameraCaptureMode
    {
        CAMERA_DEVID = 0,
        CAMERA_URI = 1
    };

    class Camera
    {
    private:
        void grabber();
        std::string _location;
        int _device;
        std::mutex _img_lock;
        cv::Mat _img;
        cv::VideoCapture _cam;
        std::mutex _cont_mtx;
        bool _cont;
        CameraCaptureMode _capmode;
    public:
        Camera(std::string location, int device, CameraCaptureMode capmode);
        Camera(Camera&& other);
        ~Camera();
        void operator=(Camera& other);
        cv::Mat get();
        cv::Mat getw();
    };

    Camera newCamera(int device);
    Camera newCamera(std::string location);

    namespace Cam0
    {
        cv::Mat get();
        cv::Mat getw();
    }
}
#endif // CAMERA_HPP
