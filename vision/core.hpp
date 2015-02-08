#ifndef CORE_HPP
#define CORE_HPP
#include <map>
#include <opencv2/core/core.hpp>
struct VisionValue
{
public:
    cv::Scalar high;
    cv::Scalar low;
    std::map<std::string, int> extval;
    VisionValue() { (*this).high = cv::Scalar(255, 255, 255); (*this).low = cv::Scalar(0, 0, 0); }
    int& operator[](std::string key) { return (*this).extval[key]; }
};

class VisionCore
{
private:
    std::map<std::string, VisionValue> _values;
public:
    VisionValue& operator[](std::string key) { return _values[key]; }
};
#endif // CORE_HPP
