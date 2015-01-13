#include "thresholdserver.hpp"
#include "../DevLib/string.hpp"
#include <cstring>
#include <string>
#include <sstream>
#include <opencv2/imgproc/imgproc.hpp>
#include "camera.hpp"
VisionServer::ThresholdServer::ThresholdServer(int port)
{
    start(port);
}

cv::Scalar VisionServer::ThresholdServer::getHigh(std::string name)
{
    return _high[name];
}

cv::Scalar VisionServer::ThresholdServer::getLow(std::string name)
{
    return _low[name];
}

VisionServer::ThresholdExtras VisionServer::ThresholdServer::getOther(std::string name)
{
    return _other[name];
}

void VisionServer::ThresholdServer::setHigh(std::string name, cv::Scalar s)
{
    _high[name] = s;
}

void VisionServer::ThresholdServer::setLow(std::string name, cv::Scalar s)
{
    _low[name] = s;
}

void VisionServer::ThresholdServer::setOther(std::string name, VisionServer::ThresholdExtras other)
{
    _other[name] = other;
}

void VisionServer::ThresholdServer::add(std::string name, cv::Scalar low, cv::Scalar high, VisionServer::ThresholdExtras other)
{
    _low[name] = low;
    _high[name] = high;
}

void VisionServer::ThresholdServer::request_handler(dev::http_session& session)
{
    if(session.path == "/")
    {
        session.status_code = 301;
        session.headers["location"] = "/wi2";
    }
    else if(session.path == "/setvalsw")
    {
        //Send back some data to the browser to automate the redirection!
        session.response = "<html><head><title>Redirection under progress</title></head><body><h1>Please wait while we redirect you!</h1></body></html>";
        session.status_code = 301;
        session.headers["location"] = "/wi2";

        //Process the request
        if(session.queries["type"] == "high")
        {
            _high[session.queries["name"]] = cv::Scalar(
                std::atof(session.queries["1"].c_str()),
                std::atof(session.queries["2"].c_str()),
                std::atof(session.queries["3"].c_str())
            );
            _other[session.queries["name"]].GaussianKernel = std::atoi(session.queries["GaussianKernel"].c_str());
            _other[session.queries["name"]].Epsilon = std::atoi(session.queries["Epsilon"].c_str());
        }
        else if(session.queries["type"] == "low")
        {
            _low[session.queries["name"]] = cv::Scalar(
                std::atof(session.queries["1"].c_str()),
                std::atof(session.queries["2"].c_str()),
                std::atof(session.queries["3"].c_str())
            );
            _other[session.queries["name"]].GaussianKernel = std::atoi(session.queries["GaussianKernel"].c_str());
            _other[session.queries["name"]].Epsilon = std::atoi(session.queries["Epsilon"].c_str());
        }
    }
    else if(session.path == "/setvals")
    {
        //Process the request
        _other[session.queries["name"]].GaussianKernel = std::atoi(session.queries["GaussianKernel"].c_str());
        _other[session.queries["name"]].Epsilon = std::atoi(session.queries["Epsilon"].c_str());
        if(session.queries["type"] == "high")
        {
            _high[session.queries["name"]] = cv::Scalar(
                std::atof(session.queries["1"].c_str()),
                std::atof(session.queries["2"].c_str()),
                std::atof(session.queries["3"].c_str())
            );
        }
        else if(session.queries["type"] == "low")
        {
            _low[session.queries["name"]] = cv::Scalar(
                std::atof(session.queries["1"].c_str()),
                std::atof(session.queries["2"].c_str()),
                std::atof(session.queries["3"].c_str())
            );
        }
        session.response = "Success!";
        session.headers["content-type"] = "text/plain";
    }
    else if(session.path == "/getvals")
    {
        std::stringstream x;
        cv::Scalar z = _low[session.queries["name"]];
        cv::Scalar y = _high[session.queries["name"]];
        session.headers["content-type"] = "text/plain";
        x << z[0] << ' ' << y[0] << ' ' << z[1] << ' ' << y[1] << ' ' << z[2] << ' ' << y[2];
    }
    else if(session.path == "/getImage")
    {
        if(session.queries["type"] == "reg")
        {
            std::vector<uchar> buffer;
            cv::imencode(".jpg", camera::Cam0::get(), buffer);
            for(unsigned int i = 0; i < buffer.size(); i++)
            {
                session.response += (char) buffer[i];
            }
            session.headers["content-type"] = "image/jpg";
        }
        else
        {
            std::vector<uchar> buffer;
            cv::Mat bufx;
            cv::cvtColor(camera::Cam0::get(), bufx, CV_BGR2HSV);
            cv::inRange(bufx, getLow(session.queries["name"]), getHigh(session.queries["name"]), bufx);
            if(getOther(session.queries["name"]).GaussianKernel < 1)
            {
                setOther(
                    session.queries["name"],
                    VisionServer::ThresholdExtras(1, getOther(session.queries["name"]).Epsilon)
                );
            }
            if(getOther(session.queries["name"]).GaussianKernel % 2 == 0)
            {
                setOther(
                    session.queries["name"],
                    VisionServer::ThresholdExtras(getOther(session.queries["name"]).GaussianKernel + 1, getOther(session.queries["name"]).Epsilon)
                );
            }
            cv::GaussianBlur(bufx, bufx, cv::Size(getOther(session.queries["name"]).GaussianKernel, getOther(session.queries["name"]).GaussianKernel), 0, 0, cv::BORDER_DEFAULT);
            cv::imencode(".jpg", bufx, buffer);
            for(unsigned int i = 0; i < buffer.size(); i++)
            {
                session.response += (char) buffer[i];
            }
            session.headers["content-type"] = "image/jpg";
        }
    }
    else if(session.path == "/wi2")
    {
        std::stringstream o;
        o << "<html><head><title>Threshold Values</title></head><body>";
        o << "<table style=\"margin: 0px;\">";
        o << "<tr><th><h2>Maximum</h2></th><th><h2>Minumum</h2></th><th><h2>Midpass</h2></th><th><h2>Unprocessed</h2></th><th><h2>Thresholded</h2></th></tr>";
        for(std::map<std::string, cv::Scalar>::const_iterator it = _high.begin(); it != _high.end(); ++it)
        {
            std::string rgbh = dev::toString(_high[it->first][0]) + ", " + dev::toString(_high[it->first][1]) + ", " + dev::toString(_high[it->first][2]);
            std::string antirgbh = dev::toString(255 - _high[it->first][0]) + ", " + dev::toString(255 - _high[it->first][1]) + ", " + dev::toString(255 - _high[it->first][2]);

            std::string rgbl = dev::toString(_low[it->first][0]) + ", " + dev::toString(_low[it->first][1]) + ", " + dev::toString(_low[it->first][2]);
            std::string antirgbl = dev::toString(255 - _low[it->first][0]) + ", " + dev::toString(255 - _low[it->first][1]) + ", " + dev::toString(255 - _low[it->first][2]);

            cv::Scalar high = _high[it->first];
            cv::Scalar low = _low[it->first];
            //Each of the rows
            o << "<tr>";
            //Maximum
            o << "<td>";
            o << "<form style=\"margin: 0px;\" action=\"/setvalsw?type=high&name=" << it->first << "\" method=\"POST\">";
            o << "<table style=\"background-color: rgb(" << rgbh << "); color: rgb(" << antirgbh << ");\">";
                o << "<tr><td>Channel 1</td><td>Channel 2</td><td>Channel 3</td></tr>";
                o << "<tr>";
                    o << "<td><input name=\"1\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << high[0] << "\"></td>";
                    o << "<td><input name=\"2\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << high[1] << "\"></td>";
                    o << "<td><input name=\"3\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << high[2] << "\"></td>";
                o << "</tr>";
                o << "<tr><td>Gaussian</td><td>Epsilon</td><td>Submit</td></tr>";
                o << "<tr>";
                    o << "<td><input name=\"GaussianKernel\" type=\"range\" min=\"1\" max=\"16\" placeholder=\"Channel 1\" value=\"" << _other[it->first].GaussianKernel << "\"></td>";
                    o << "<td><input name=\"Epsilon\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << _other[it->first].Epsilon << "\"></td>";
                    o << "<td><input type=\"submit\" value=\"Submit\"></td>";
                o << "</tr>";
            o << "</table></form>";
            o << "</td>";
            //Minimum
            o << "<td>";
            o << "<form style=\"margin: 0px;\" action=\"/setvalsw?type=low&name=" << it->first << "\" method=\"POST\">";
            o << "<table style=\"background-color: rgb(" << rgbl << "); color: rgb(" << antirgbl << ");\">";
                o << "<tr><td>Channel 1</td><td>Channel 2</td><td>Channel 3</td></tr>";
                o << "<tr>";
                    o << "<td><input name=\"1\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << low[0] << "\"></td>";
                    o << "<td><input name=\"2\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << low[1] << "\"></td>";
                    o << "<td><input name=\"3\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << low[2] << "\"></td>";
                o << "</tr>";
                o << "<tr><td>Gaussian</td><td>Epsilon</td><td>Submit</td></tr>";
                o << "<tr>";
                    o << "<td><input name=\"GaussianKernel\" type=\"range\" min=\"1\" max=\"16\" placeholder=\"Channel 1\" value=\"" << _other[it->first].GaussianKernel << "\"></td>";
                    o << "<td><input name=\"Epsilon\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << _other[it->first].Epsilon << "\"></td>";
                    o << "<td><input type=\"submit\" value=\"Submit\"></td>";
                o << "</tr>";
            o << "</table></form>";
            o << "</td>";
            //Midpass
            o << "<td style=\"border: 2px solid #404040; background-color: rgb(" << (int) (high[0] + low[0]) / 2 << ", " << (int) (high[1] + low[1]) / 2 << ", " << (int) (high[2] + low[2]) / 2 << ");\">";
            o << "</td>";
            //Unprocessed
            o << "<td>";
            o << "<img style=\"border: 2px solid #404040; height: 90px;\" src=\"/getImage?type=reg&name=" << it->first << "\"></img>";
            o << "</td>";
            //Thresholded
            o << "<td>";
            o << "<img style=\"border: 2px solid #404040; height: 90px;\" src=\"/getImage?type=thresh&name=" << it->first << "\"></img>";
            o << "</td>";
            //END
            o << "</tr>";
        }
        o << "</table>";
        o << "</body></html>";
        session.response = o.str();
    }
}
