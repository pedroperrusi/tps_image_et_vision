#include <stdio.h>
#include <opencv2/opencv.hpp>

int main( int argc, char** argv )
{
    cv::Mat img;
    img = cv::imread( "data/lena.jpg" );
    if( !img.data )
    {
        printf("No image data\n");
        return -1;
    }

    cv::Mat grey;
    cv::cvtColor(img, grey, cv::COLOR_RGB2GRAY);

    cv::imshow("Image", img);
    cv::imshow( "Grey Image", grey);
    cv::waitKey(0);

    return 0;
}