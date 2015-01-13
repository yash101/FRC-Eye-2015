#include "visionalgs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
std::vector<cv::Rect> VisionServer::findYellowTotes(cv::Mat image, cv::Scalar high, cv::Scalar low)
{
    cv::Mat workingImage;
    cv::cvtColor(image, workingImage, CV_BGR2HSV);
    cv::inRange(workingImage, low, high, workingImage);
    //cv::GaussianBlur()
}

std::vector<cv::Rect> VisionServer::findGrayTotes(cv::Mat image, cv::Scalar high, cv::Scalar low)
{
    cv::Mat workingImage;
    cv::cvtColor(image, workingImage, CV_BGR2HSV);
}

std::vector<cv::Rect> VisionServer::findPoolNoodles(cv::Mat image, cv::Scalar high, cv::Scalar low)
{
    cv::Mat workingImage;
    cv::cvtColor(image, workingImage, CV_BGR2HSV);
}

std::vector<cv::Rect> VisionServer::findGarbageCans(cv::Mat image, cv::Scalar high, cv::Scalar low)
{
    cv::Mat workingImage;
    cv::cvtColor(image, workingImage, CV_BGR2HSV);
}

std::string VisionServer::toString(std::vector<cv::Rect> rectangles)
{
    std::stringstream x;
    for(unsigned int i = 0; i < rectangles.size(); i++)
    {
        x << rectangles[i].x << ' ' << rectangles[i].y << ' ' << rectangles[i].width << ' ' << rectangles[i].height << ' ';
    }
    return x.str();
}
