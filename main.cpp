#include "src/visionserver.hpp"
#include "src/thresholdserver.hpp"
#include "src/log.hpp"
#include "DevLib/Servers/http.hpp"
#include <thread>
#include <signal.h>
#include "DevLib/math.hpp"
#include <iomanip>
#include "src/camera.hpp"

void x(int x)
{
    exit(EXIT_SUCCESS);
}

int main()
{
//    std::mutex hehe;
//    std::mutex hehe2;
//    for(int h = 0; h < 12; h++)
//    {
//        int hehe3 = 0;
//        for(int i = 0; i < 16; i++)
//        {
//            hehe2.lock();
//            hehe3++;
//            hehe2.unlock();
//            std::thread([&](int j)
//            {
//                hehe.lock();
//                std::cout << "[Thread " << j << "][INIT]" << std::endl;
//                hehe.unlock();
//                long long tm1 = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
//                long double z = dev::piCalc<long double>(500000000);
//                long long tm2 = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
//                hehe.lock();
//                std::cout << "[Thread " << j << "][PI][" << std::setprecision(200) << z << "][500 million iterations in " << tm2 - tm1 << " nanoseconds ";
//                std::cout << "(" << std::setprecision(9) << (long double) (tm2 - tm1) / (long double) 10e8 << " seconds)!]" << std::endl;
//                hehe.unlock();
//                hehe2.lock();
//                hehe3--;
//                hehe2.unlock();
//            }, i).detach();
//        }
//        while(hehe3 > 0) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
//    }
//    return 0;




    signal(SIGINT, x);
    signal(SIGTERM, x);
    signal(SIGSEGV, x);
    signal(SIGCHLD, x);
    VisionServer::ThresholdServer x(1234);
    x.add("1", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("2", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("3", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("4", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("5", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("6", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("7", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("8", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    x.add("9", cv::Scalar(0,0,0), cv::Scalar(2,2,2), VisionServer::ThresholdExtras(1, 1));
    tool::log log("log");
    log.put("Hello World!");
    log.put(102.345223);
    dev::lambda_http_server([](dev::http_session& session)
    {
        std::stringstream x;
        x << "<html><body><form action=\"/\" method=\"post\" enctype=\"multipart/form-data\">";
        x << "<p><input type=\"text\" name=\"text\" value=\"text default\">";
        x << "<p><input type=\"file\" name=\"file1\">";
        x << "<p><input type=\"file\" name=\"file2\">";
        x << "<p><button type=\"submit\">Submit</button>";
        x << "<p>file1: [<pre>" << session.files["file1"] << "</pre>]</p>";
        x << "<p>file2: [<pre>" << session.files["file2"] << "</pre>]</p>";
        x << "</form></body></html>";
        session.response = x.str();
    }).start(12345);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1024));
    }
}
