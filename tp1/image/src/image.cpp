#include <stdio.h>
#include <opencv2/opencv.hpp>

/*
    Exercise 2: Affichage d'une image
*/
int main( int argc, char** argv )
{
    cv::Mat img;
    img = cv::imread( "data/lena.jpg" );
    if( !img.data )
    {
        printf("No image data\n");
        return -1;
    }
    cv::namedWindow( "Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Display Image", img );
    cv::waitKey(0);
    return 0;
}