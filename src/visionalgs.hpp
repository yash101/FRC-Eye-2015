#ifndef VISIONALGS_HPP
#define VISIONALGS_HPP
#include <opencv2/core/core.hpp>
#include "thresholdserver.hpp"
#include <vector>
namespace VisionServer
{
    std::vector<cv::Rect> findPolygons(std::string name, ::VisionServer::ThresholdServer& thresholdHost);
//    std::vector<cv::Rect> findGrayTotes(cv::Mat image, cv::Scalar high, cv::Scalar low);
//    std::vector<cv::Rect> findPoolNoodles(cv::Mat image, cv::Scalar high, cv::Scalar low);
//    std::vector<cv::Rect> findGarbageCans(cv::Mat image, cv::Scalar high, cv::Scalar low);
//    std::vector<cv::Rect> findGarbageCanLid(cv::Mat image);
    std::string toString(std::vector<cv::Rect> rectangles);
}
#endif // VISIONALGS_HPP
