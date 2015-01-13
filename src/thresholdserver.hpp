#ifndef THRESHOLDSERVER_HPP
#define THRESHOLDSERVER_HPP
#include "../DevLib/Servers/http.hpp"
#include <opencv2/core/core.hpp>
#include <mutex>
#include <map>
namespace VisionServer
{
    class ThresholdExtras
    {
    public:
        int GaussianKernel;
        int Epsilon;
        ThresholdExtras(int Gaussian, int Eps) : GaussianKernel(Gaussian), Epsilon(Eps) {}
        ThresholdExtras() : GaussianKernel(0), Epsilon(0) {}
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

//    class ThresholdServer : public dev::http_server
//    {
//    private:
//        cv::Scalar values;
//        cv::Scalar values2;
//        void request_handler(dev::http_session& session);
//        std::mutex valueMutex;
//    public:
//        ThresholdServer(int port);
//        cv::Scalar getThresholdLow()
//        {
//            valueMutex.lock();
//            cv::Scalar x(values[0], values[1], values[2]);
//            valueMutex.unlock();
//            return x;
//        }

//        cv::Scalar getThresholdHigh()
//        {
//            valueMutex.lock();
//            cv::Scalar x(values2[0], values2[1], values2[2]);
//            valueMutex.unlock();
//            return x;
//        }

//        void setThresholdValues(cv::Scalar x, cv::Scalar y)
//        {
//            valueMutex.lock();
//            values = cv::Scalar(x[0], x[1], x[2]);
//            values2 = cv::Scalar(y[0], y[1], y[2]);
//            valueMutex.unlock();
//        }
//    };
}
#endif // THRESHOLDSERVER_H
