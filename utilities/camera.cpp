#include "camera.hpp"
#include <thread>
camera::Camera::Camera(std::string location, int device, CameraCaptureMode capmode)
:
    _location(location),
    _device(device),
    _cont(true),
    _capmode(capmode)
{
    if(_capmode == CAMERA_DEVID) { _cam = cv::VideoCapture(_device); }
    else if(_capmode == CAMERA_URI) { _cam = cv::VideoCapture(_location); }
    std::thread(&camera::Camera::grabber, this).detach();
}

void camera::Camera::grabber()
{
    cv::Mat tmpmat;
    if(_cam.isOpened())
    {
        //Infinitely grab images from the camera
        while(true)
        {
            //Check to see if someone wants us to quit this thread
            _cont_mtx.lock();
            bool x = _cont;
            _cont_mtx.unlock();
            if(!x)
            {
                _cam.release();
                return;
            }

            //Grab an image and send it to the global image
            _cam >> tmpmat;
            if(!tmpmat.empty())
            {
                _img_lock.lock();
                _img = tmpmat.clone();
                _img_lock.unlock();
            }
        }
    }
}

cv::Mat camera::Camera::get()
{
    _img_lock.lock();
    cv::Mat x = _img.clone();
    _img_lock.unlock();
    return x;
}

cv::Mat camera::Camera::getw()
{
    cv::Mat x;
    do
    {
        x = get();
        if(x.empty()) { std::this_thread::sleep_for(std::chrono::milliseconds(2)); }    //Sleep a bit to prevent swamping the CPU
    }
    while(x.empty());
    return x;
}

camera::Camera::~Camera()
{
    _cont_mtx.lock();
    _cont = false;
    _cont_mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    _img_lock.unlock();
}

void camera::Camera::sendStopSignal()
{
    _cont_mtx.lock();
    _cont = false;
    _cont_mtx.unlock();
    _cam.release();
}

camera::Camera camera::newCamera(int device) { return camera::Camera("", device, CAMERA_DEVID); }
camera::Camera camera::newCamera(std::string location) { return camera::Camera(location, -1, CAMERA_URI); }

#ifdef CAMERA_DEFAULT_ENABLE
static camera::Camera Cam0 = ::camera::newCamera(CAMERA_DEFAULT_INITIALIZER);
#endif

cv::Mat camera::Cam0::get() { return ::Cam0.get(); }
cv::Mat camera::Cam0::getw() { return ::Cam0.getw(); }
void camera::Cam0::stop() { ::Cam0.sendStopSignal(); }
