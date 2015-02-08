#ifndef VALUESERVER2_HPP
#define VALUESERVER2_HPP
#include "../DevLib/Servers/http.hpp"
#include "../DevLib/configuration.hpp"
#include <map>
#include <vector>
#include <string>
#include <opencv2/core/core.hpp>

struct VisionValues
{
public:
    std::vector<double> threshold_high;
    std::vector<double> threshold_low;
    int threshMode;
    int gaussianKernel;
    int epsilon;
    int hessian;

    VisionValues() {}
    VisionValues(std::vector<double> high, std::vector<double> low, int mode, int gaussian, int targetEpsilon)
        : threshold_high(high), threshold_low(low), threshMode(mode), gaussianKernel(gaussian), epsilon(targetEpsilon)
    {}
    VisionValues(cv::Scalar high, cv::Scalar low, int mode, int gaussian, int targetEpsilon, int _hessian)
        : threshMode(mode), gaussianKernel(gaussian), epsilon(targetEpsilon), hessian(_hessian)
    {
        for(size_t i = 0; i < high.channels; i++) threshold_high.push_back(high[i]);
        for(size_t i = 0; i < low.channels; i++) threshold_low.push_back(low[i]);
    }
    void threshold(std::string loadFrom, cv::Mat& image);
    void setHighThreshold(std::vector<double> high) { threshold_high = high; }
    void setLowThreshold(std::vector<double> low) { threshold_low = low; }
    void setHighThreshold(cv::Scalar high)
        { threshold_high.clear(); for(size_t i = 0; i < high.channels; i++) threshold_high.push_back(high[i]); }
    void setLowThreshold(cv::Scalar low)
        { threshold_low.clear(); for(size_t i = 0; i < low.channels; i++) threshold_low.push_back(low[i]); }
    void setThreshMode(int mode) { threshMode = mode; }
    void setGaussianKernel(int kernelWidth) { gaussianKernel = kernelWidth; }
    void setEpsilon(int _epsilon) { epsilon = _epsilon; }
    void setHessian(int _hessian) { hessian = _hessian; }
    int getThreshMode() { return threshMode; }
    int getGaussianKernel() { return gaussianKernel; }
    int getEpsilon() { return epsilon; }
    int getHessian() { return hessian; }
};

class ValueServer2 : public dev::http_server
{
private:
    std::map<std::string, VisionValues> _values;
    std::string _storagefn;
    dev::ConfigurationParser _valstor;
    std::vector<std::string> _namelist;
public:
    ValueServer2(std::string storagefn, int port);
    void request_handler(dev::http_session& session);
    void add(std::string key, VisionValues value) { _values[key] = value; }
    VisionValues get(std::string key) { return _values[key]; }
    void remove(std::string key) { _values.erase(key); }
    VisionValues& operator[](std::string key) { return _values[key]; }
};

#endif // VALUESERVER2_HPP
