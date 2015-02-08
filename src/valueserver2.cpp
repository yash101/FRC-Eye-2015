#include "valueserver2.hpp"
#include "../DevLib/configuration.hpp"
#include "../DevLib/string.hpp"
#include <opencv2/imgproc/imgproc.hpp>

//Storage Algorithm:
//ValueDB=field1>0,1,2,4:field2>hi
ValueServer2::ValueServer2(std::string storagefn, int port) : _storagefn(storagefn), _valstor(storagefn)
{
    start(port);
    if(_valstor.tryBool("usefilesystem"))
    {
        std::stringstream names(_valstor["db_names"]);
        std::string buffer;
        while(std::getline(names, buffer, ','))
        {
            _namelist.push_back(buffer);
        }
        for(size_t i = 0; i < _namelist.size(); i++)
        {
            dev::ConfigurationParser curcfg(_storagefn + "." + _namelist[i] + ".db");
            _values[_namelist[i]] = VisionValues(
                cv::Scalar(
                    dev::stof(curcfg["ch1_mx"]),
                    dev::stof(curcfg["ch2_mx"]),
                    dev::stof(curcfg["ch3_mx"])
                ),
                cv::Scalar(
                    dev::stof(curcfg["ch1_mn"]),
                    dev::stof(curcfg["ch2_mn"]),
                    dev::stof(curcfg["ch2_mn"])

                ),
                dev::stoi(curcfg["colorspace"]),
                dev::stoi(curcfg["gaussian"]),
                dev::stoi(curcfg["epsilon"]),
                dev::stoi(curcfg["minHessian"])
            );
        }
    }
}

void ValueServer2::request_handler(dev::http_session& session)
{
    if(session.path == "/")
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
                o << "<tr><td>Name</td><td>Channel 1</td><td>Channel 2</td><td>Channel 3</td></tr>";
                o << "<tr>";
                    //The name of the field
                    o << "<td>" << it->first << "</td>";
                    o << "<td><input name=\"1\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << high[0] << "\"></td>";
                    o << "<td><input name=\"2\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << high[1] << "\"></td>";
                    o << "<td><input name=\"3\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << high[2] << "\"></td>";
                o << "</tr>";
                o << "<tr><td>Gaussian</td><td>Epsilon</td><td>Color Space<td>Submit</td></tr>";
                o << "<tr>";
                    o << "<td><input name=\"GaussianKernel\" type=\"range\" min=\"1\" max=\"16\" placeholder=\"Channel 1\" value=\"" << _other[it->first].GaussianKernel << "\"></td>";
                    o << "<td><input name=\"Epsilon\" type=\"range\" min=\"0\" max=\"255\" placeholder=\"Channel 1\" value=\"" << _other[it->first].Epsilon << "\"></td>";
                    o << "<td><select name=\"ColorSpace\">";
                        o << "<option value=\"HSV\" " << ((VisionServer::toCV(getOther(it->first).cvtColor) == CV_BGR2HSV) ? "selected" : "") << "\">HSV</option>";
                        o << "<option value=\"BGR\" " << ((VisionServer::toCV(getOther(it->first).cvtColor) == -1) ? "selected" : "") << ">BGR</option>";
                        o << "<option value=\"RGB\" " << ((VisionServer::toCV(getOther(it->first).cvtColor) == CV_BGR2RGB) ? "selected" : "") << ">RGB</option>";
                        o << "<option value=\"GRAY\" " << ((VisionServer::toCV(getOther(it->first).cvtColor) == CV_BGR2GRAY) ? "selected" : "") << ">GRAY</option>";
                    o << "</select></td>";
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


//_values[_namelist[i]] = VisionValues(cv::Scalar(dev::stof(curcfg["ch1_mx"]),dev::stof(curcfg["ch2_mx"]),dev::stof(curcfg["ch3_mx"])),cv::Scalar(dev::stof(curcfg["ch1_mn"]),dev::stof(curcfg["ch2_mn"]),dev::stof(curcfg["ch2_mn"])),dev::stoi(curcfg["colorspace"]),dev::stoi(curcfg["gaussian"]),dev::stoi(curcfg["epsilon"]),dev::stoi(curcfg["minHessian"]));
