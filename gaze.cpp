#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv )
{
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
