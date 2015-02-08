#include "visionalgs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "camera.hpp"

//------------------------[Finds Polygons]---------------------------
//This algorithm grabs a frame from the default camera and then processes it.
//  1.  Convert the image to the HSV color space
//  2.  Threshold the image for all of it's three channels
//  3.  Smooth the image using a Gaussian Transformation
//  4.  Extract vertices of possible polygons from the image
//  5.  Approximate polygonomal curves with a specified accuracy, modifyable via the
//          online configuration utility
//  6.  Check the convexity of the polygons. This rids the search of a lot of noise
//  7.  Populate a std::vector<cv::Rect> of the bounding rectangles. This is the array
//          return from this function
std::vector<cv::Rect> VisionServer::findPolygons(std::string name, VisionServer::ThresholdServer& thresh)
{
    //******PREPARE FOR SEARCH*******
    //This is our working image
    cv::Mat workingImage = camera::Cam0::getw();

    //Convert the image to the wanted color space
    if(VisionServer::toCV(thresh.getOther(name).cvtColor) != -1)
    {
        cv::cvtColor(workingImage, workingImage, VisionServer::toCV(thresh.getOther(name).cvtColor));
    }

    //Threshold our image with our threshold values
    cv::inRange(workingImage, thresh.getLow(name), thresh.getHigh(name), workingImage);

    //Validate the Gaussian Kernel before using it!
    //Ensure that the kernel isn't less than one. If it is, correct it!
    VisionServer::ThresholdExtras x = thresh.getOther(name);
    if(x.GaussianKernel < 1) x.GaussianKernel = 1;
    if(x.GaussianKernel % 2 == 0) x.GaussianKernel++;
    thresh.setOther(name, x);
    //Use a Gaussian Blur to clean up the threshold
    cv::GaussianBlur(
        workingImage,
        workingImage,
        cv::Size(
            thresh.getOther(name).GaussianKernel,
            thresh.getOther(name).GaussianKernel
        ),
        0,
        0,
        cv::BORDER_DEFAULT
    );

    //Convert the image to a binary image (presumably will cause higher performance)
    cv::threshold(workingImage, workingImage, 128, 255, CV_THRESH_BINARY);

    //*******START THE SEARCH********
    //This stores all of the contour vertices
    std::vector<std::vector<cv::Point> > contours;
    //This is for the hierarchy
    std::vector<cv::Vec4i> hierarchy;
    //Find the contours in the image
    cv::findContours(workingImage, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    //Stores our polygons
    std::vector<std::vector<cv::Point> > poly(contours.size());
    //Stores our bounding rectangles (To be returned)
    std::vector<cv::Rect> bound_rect;

    //Iterate through all of the contours
    for(size_t i = 0; i < contours.size(); i++)
    {
        if(cv::contourArea(contours[i]) < 16) continue;
        //Convert the vertices to polygons
        cv::approxPolyDP(contours[i], poly[i], thresh.getOther(name).Epsilon, true);

        //Check the convexity of the polygon and skip this polygon if convexity isn't great!
        if(!cv::isContourConvex(poly[i])) continue;

        //Add this to our bounding rectangles
        bound_rect.push_back(cv::boundingRect(cv::Mat(poly[i])));
    }

    //Return our search results
    return bound_rect;
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
