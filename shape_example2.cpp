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

static void help()
{
    printf("\n"
            "This program demonstrates a method for shape comparisson based on Shape Context\n"
            "You should run the program providing a number between 1 and 20 for selecting an image in the folder ../data/shape_sample.\n"
            "Call\n"
            "./shape_example [number between 1 and 20, 1 default]\n\n");
}

static vector<Point> simpleContour( const Mat& currentQuery, int n=300 )
{
    vector<vector<Point> > _contoursQuery;
    vector <Point> contoursQuery;
    findContours(currentQuery, _contoursQuery, RETR_LIST, CHAIN_APPROX_NONE);
    for (size_t border=0; border<_contoursQuery.size(); border++)
    {
        for (size_t p=0; p<_contoursQuery[border].size(); p++)
        {
            contoursQuery.push_back( _contoursQuery[border][p] );
        }
    }

    // In case actual number of points is less than n
    int dummy=0;
    for (int add=(int)contoursQuery.size()-1; add<n; add++)
    {
        contoursQuery.push_back(contoursQuery[dummy++]); //adding dummy values
    }

    // Uniformly sampling
    random_shuffle(contoursQuery.begin(), contoursQuery.end());
    vector<Point> cont;
    for (int i=0; i<n; i++)
    {
        cont.push_back(contoursQuery[i]);
    }
    return cont;
}

int main(int argc, char** argv)
{
    string path = "data/";
    cv::CommandLineParser parser(argc, argv, "{help h||}{@input|1|}{@data|2|}");
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string indexQuery = parser.get<string>("@input");
    int data = parser.get<int>("@data");
    if (!parser.check())
    {
        parser.printErrors();
        help();
        return 1;
    }
    // if (indexQuery < 1 || indexQuery > 20)
    // {
    //     help();
    //     return 1;
    // }
    cv::Ptr <cv::ShapeContextDistanceExtractor> mysc = cv::createShapeContextDistanceExtractor();

    Size sz2Sh(300,300);
    stringstream queryName;
    queryName<<indexQuery;
    Mat query=imread(queryName.str(), IMREAD_GRAYSCALE);
    Mat color=imread(queryName.str());
    int npoints = 0;

    //BW Operation
    GaussianBlur(query, query, Size(7,7), 1.5, 1.5);
    GaussianBlur(color, color, Size(7,7), 1.5, 1.5);
    threshold(query, query, 200, 255, CV_THRESH_BINARY);
    bitwise_not ( query, query );

    // Mat queryToShow;
    // resize(query, queryToShow, sz2Sh);
    // imshow("QUERY", queryToShow);
    vector<Point> contQuery = simpleContour(query);

    //set the size of the contour
    npoints = contQuery.size();
    // Create the mask with the polygon
    Mat1b mask = cv::Mat1b::zeros(color.rows,color.cols);
    // Mat1b colorViz = cv::Mat1b::zeros(color.rows,color.cols);
    //This is required for converting Vector Points to Point*
    const Point* elementPoints[1] = { &contQuery[0] };

    // //Show the colored image
    // std::cout << "Color image" << std::endl;
    // imshow("ColorViz", color);
    // moveWindow("ColorViz", sz2Sh.width+250,sz2Sh.height+100);
    // waitKey(30);
    // getchar();
    // //Show the blank mask
    // std::cout << "Blank Mask" << std::endl;
    // imshow("ColorViz", mask);
    // moveWindow("ColorViz", sz2Sh.width+250,sz2Sh.height+100);
    // waitKey(30);
    // getchar();

    //Mat1b mask(color.rows, color.cols, uchar(0));
    //Fill the mask with the shape of the pant
    fillPoly(mask, elementPoints, &npoints, 1, Scalar(255));

    // //Show the masked image with the pant
    // std::cout << "Filled Mask" << std::endl;
    // imshow("ColorViz", mask);
    // moveWindow("ColorViz", sz2Sh.width+250,sz2Sh.height+100);
    // waitKey(30);
    // getchar();

    // Compute the mean with the computed mask
    Scalar average = mean(color, mask);

    //Fill in color poly
    // fillPoly(colorViz, elementPoints, &npoints, 1, Scalar(average[0], average[1], average[2]));

    // //Show the colored mask
    // std::cout << "Color Mask" << std::endl;
    // imshow("ColorViz", colorViz);
    // moveWindow("ColorViz", sz2Sh.width+250,sz2Sh.height+100);
    // waitKey(30);
    // getchar();

    // std::cout << "Color is: " << average << std::endl;

    int bestMatch = 0;
    float bestDis=FLT_MAX;
    for ( int ii=1; ii<=data; ii++ )
    {
        //waitKey(30);
        stringstream iiname;
        iiname<<path<<ii<<".png";
        Mat iiIm=imread(iiname.str(), 0);
        // Mat iiToShow;
        // resize(iiIm, iiToShow, sz2Sh);
        // imshow("TEST", iiToShow);
        // moveWindow("TEST", sz2Sh.width+50,0);
        vector<Point> contii = simpleContour(iiIm);
        float dis = mysc->computeDistance( contQuery, contii );
        if ( dis<bestDis )
        {
            bestMatch = ii;
            bestDis = dis;
        }
        // std::cout<<" distance between "<<queryName.str()<<" and "<<iiname.str()<<" is: "<<dis<<std::endl;
        // stringstream bestname;
        // bestname<<path<<bestMatch<<".png";
        // Mat iiIm2=imread(bestname.str(), 0);
        // Mat bestToShow;
        // resize(iiIm2, bestToShow, sz2Sh);
        // imshow("BEST MATCH", bestToShow);
        // moveWindow("BEST MATCH", sz2Sh.width+450,0);
    }
    // ofstream myfile;
    // myfile.open ("answer.txt");
    switch(bestMatch) {
        case 1: std::cout<< "pant" <<std::endl;break;
        case 2: std::cout<< "pant" <<std::endl;break;
        case 3: std::cout<< "shirt" <<std::endl;break;
        case 4: std::cout<< "shirt" <<std::endl;break;
        case 5: std::cout<< "shirt" <<std::endl;break;
        case 6: std::cout<< "shirt" <<std::endl;break;
        case 7: std::cout<< "shirt" <<std::endl;break;
        case 8: std::cout<< "shirt" <<std::endl;break;
        case 9: std::cout<< "shirt" <<std::endl;break;
        case 10: std::cout<< "shirt" <<std::endl;break;
        case 11: std::cout<< "pant" <<std::endl;break;
        case 12: std::cout<< "pant" <<std::endl;break;
    }
    //getchar();
    std::cout<< (int)average[0] << "," << (int)average[1] << "," << (int)average[2] <<std::endl;
    //myfile.close();

    return 0;
}
