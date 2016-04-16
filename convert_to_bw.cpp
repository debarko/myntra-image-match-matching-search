/*
 * shape_context.cpp -- Shape context demo for shape matching
 */

#include "opencv2/shape.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	cv::CommandLineParser parser(argc, argv, "{help h||}{@input|1|}");
    string indexQuery = parser.get<string>("@input");
    Mat a_image = imread(indexQuery);

    cvtColor(a_image, a_image, CV_BGR2GRAY);
    GaussianBlur(a_image, a_image, Size(7,7), 1.5, 1.5);
    threshold(a_image, a_image, 200, 255, CV_THRESH_BINARY);
    imwrite("a.png", a_image);
    bitwise_not ( a_image, a_image );
    imwrite("b.png", a_image);

    return 0;
}
