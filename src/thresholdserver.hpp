#ifndef THRESHOLDSERVER_HPP
#define THRESHOLDSERVER_HPP
#include "../DevLib/Servers/http.hpp"
#include <opencv2/core/core.hpp>
#include <mutex>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
enum ColorType
{
    HSV = CV_BGR2HSV,
    RGB = CV_BGR2RGB,
    BGR = -1,
    GRAY = CV_BGR2GRAY
};

namespace VisionServer
{
    ColorType fromString(std::string x);
    int toCV(ColorType x);

    class ThresholdExtras
    {
    public:
        int GaussianKernel;
        int Epsilon;
        ColorType cvtColor;

        ThresholdExtras(int Gaussian, int Eps) : GaussianKernel(Gaussian), Epsilon(Eps) {}
        ThresholdExtras() : GaussianKernel(0), Epsilon(0), cvtColor(HSV) {}
    };

    class ThresholdServer : public dev::http_server
    {
    private:
        std::map<std::string, cv::Scalar> _high;
        std::map<std::string, cv::Scalar> _low;
        std::map<std::string, VisionServer::ThresholdExtras> _other;
    public:
        ThresholdServer(int port);
        void setHigh(std::string name, cv::Scalar s);
        cv::Scalar getHigh(std::string name);
        void setLow(std::string name, cv::Scalar s);
        cv::Scalar getLow(std::string name);
        void setOther(std::string name, VisionServer::ThresholdExtras other);
        VisionServer::ThresholdExtras getOther(std::string name);
        void request_handler(dev::http_session& session);
        void add(std::string name, cv::Scalar a, cv::Scalar b, VisionServer::ThresholdExtras other);
    };
}
#endif // THRESHOLDSERVER_H
