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

    // Q4: Perform distance transform
    std::cout << "Performing image distance computation... \nPress q to continue" << std::endl;
    cv::Mat imgDistance, labels;
    cv::distanceTransform(imgThresh, imgDistance, cv::DIST_L2, 3);
    cv::imshow("Distance Transform Image", imgDistance);
    while(char c = cv::waitKey(0) != 'q');

    // Q5: Normalize distance images
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    std::cout << "Performing distance normalisation... \nPress q to continue" << std::endl;
    cv::normalize(imgDistance, imgDistance, 0, 1., cv::NORM_MINMAX);
    cv::imshow("Distance Transform Image", imgDistance);
    while(char c = cv::waitKey(0) != 'q');

    // Q6: Binariser la carte de distances sur un threshold de 0.5
    std::cout << "Performing la binarization de l'image de distances... \nPress q to continue" << std::endl;
    cv::threshold(imgDistance, imgDistance, 0.5, 1, cv::THRESH_BINARY);
    cv::imshow("Distance Transform Image", imgDistance);
    while(char c = cv::waitKey(0) != 'q');

    return 0;
}