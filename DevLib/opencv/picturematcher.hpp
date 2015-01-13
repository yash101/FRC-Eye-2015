#ifndef PICTUREMATCHER_HPP
#define PICTUREMATCHER_HPP
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <vector>
#include "../point.hpp"
#include "../vector.hpp"
#include "../Exceptions.hpp"
namespace dev
{
    const int DEFAULT_MIN_DISTANCE = 0;
    const int DEFAULT_MAX_DISTANCE = 100;
    class PictureMatcher
    {
    private:
        cv::Mat _key;
        cv::Mat _img_descriptor_key;
        int _hessian;
        cv::SurfDescriptorExtractor _detector;
        cv::SurfDescriptorExtractor _extractor;
        std::vector<cv::KeyPoint> _keypoints_key;
        double _min_dist;
        double _max_dist;
    public:
        PictureMatcher(cv::Mat& in, int hessian, double min_dist, double max_dist);
        cv::Point find(cv::Mat& locate);
        dev::Vec2i getSize();
        int getWidth();
        int getHeight();
    };
}
#endif // PICTUREMATCHER_HPP
