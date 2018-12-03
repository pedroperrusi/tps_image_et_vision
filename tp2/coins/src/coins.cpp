#include <iostream>
#include <opencv2/opencv.hpp>

/*
    On souhaite compter automatiquement le nombre de coins sur l'image.
*/
int main()
{
    // Load image of lena
    cv::Mat img = cv::imread( "data/coins.jpg" );
    if( !img.data )
    {
        printf("No image data\n");
        return -1;
    }
    // Q1: Show image
    std::cout << "Showing original image... \nPress q to continue" << std::endl;
    cv::imshow("Image", img);
    while(char c = cv::waitKey(0) != 'q');

    // Q2: Gray image
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);
    std::cout << "Showing gray image... \nPress q to continue" << std::endl;
    cv::imshow("Gray Image", imgGray);
    while(char c = cv::waitKey(0) != 'q');

    // Q3: Apply threshold to image
    std::cout << "Applying threshold to image... \nPress q to continue" << std::endl;
    cv::Mat imgThresh;
    cv::threshold(imgGray, imgThresh, 40, 255, cv::THRESH_BINARY);
    cv::imshow("Thresh Image", imgThresh);
    while(char c = cv::waitKey(0) != 'q');
    
    return 0;
}