#include "picturematcher.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/flann/flann.hpp>
#include "../string.hpp"

dev::PictureMatcher::PictureMatcher(cv::Mat& in, int hessian, double min_dist, double max_dist)
    : _key(in.clone()), _hessian(hessian), _detector(hessian), _min_dist(min_dist), _max_dist(max_dist)
{
    _detector.detect(_key, _keypoints_key);
    _extractor.compute(_key, _keypoints_key, _img_descriptor_key);
}

cv::Point dev::PictureMatcher::find(cv::Mat& locate)
{
    std::vector<cv::KeyPoint> keypoints_locate;
    _detector.detect(locate, keypoints_locate);
    cv::Mat img_descriptor_locate;
    _extractor.compute(locate, keypoints_locate, img_descriptor_locate);
    std::vector<cv::DMatch> matches;

    cv::FlannBasedMatcher matcher;

    try
    {
        matcher.match(_img_descriptor_key, img_descriptor_locate, matches);
    }
    catch(std::exception& e)
    {
        throw dev::AIException("DEVLIB PICTUREMATCHER ERROR. Unable to match images due to thrown exception: " + dev::toString(e.what()));
    }

    double max_dist = _min_dist;
    double min_dist = _max_dist;
    for(int i = 0; i < _img_descriptor_key.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
        {
            min_dist = dist;
        }
        if(dist > max_dist)
        {
            max_dist = dist;
        }
    }

    std::vector<cv::DMatch> good_matches;

    for(int i = 0; i < _img_descriptor_key.rows; i++)
    {
        if(matches[i].distance < 3 * min_dist)
        {
            good_matches.push_back(matches[i]);
        }
    }

    std::vector<cv::Point2i> object;
    std::vector<cv::Point2i> scene;

    for(unsigned int i = 0; i < good_matches.size(); i++)
    {
        object.push_back(_keypoints_key[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints_locate[good_matches[i].trainIdx].pt);
    }

    cv::Mat homography;
    try
    {
        homography = cv::findHomography(object, scene, CV_RANSAC);
    }
    catch(std::exception& e)
    {
        throw dev::AIException("DEVLIB PICTUREMATCHER ERROR. Unable to match images due to thrown exception from cv::findHomography(): " + dev::toString(e.what()));
    }

//    std::vector<cv::Point> x;
//    return x;
}
