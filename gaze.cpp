#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/lambda/lambda.hpp>
#include <iterator>
#include <algorithm>

int main(int argc, char** argv )
{
    using namespace boost::lambda;
    std::for_each(in(std::cin), in(), std::cout << (_1 * 3) << " " );

    cv::Mat frame;
    cv::VideoCapture videoCapture;
    videoCapture.open(0);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );

    for ( int i = 0; i < 100 ; ++i )
    {
        videoCapture >> frame;
        if (!frame.empty())
        {
            cv::imshow("Display Image", frame);
            cv::waitKey(0);
        }
    }
    return 0;
}
