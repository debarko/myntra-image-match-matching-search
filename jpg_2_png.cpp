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

    imwrite("q.png", a_image);

    return 0;
}
