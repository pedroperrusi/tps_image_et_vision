#include <iostream>
#include <opencv2/opencv.hpp>

/*
    Operations des Images
*/
int main()
{
    // Load image of lena
    cv::Mat img;
    img = cv::imread( "data/lena.jpg" );
    if( !img.data )
    {
        printf("No image data\n");
        return -1;
    }
    // Show image
    std::cout << "Showing original image... \nPress n to continue" << std::endl;

    cv::imshow("Image", img);
    while(char c = cv::waitKey(0) != 'n');


    // Apply gaussian blur
    int kernel_size;
    double sigmaX;
    std::cout << "Applying gaussian blur... \n input kernel size (recommends odd values):\n-> ";
    std::cin >> kernel_size;
    std::cout << "input sigmaX:\n-> ";
    std::cin >> sigmaX;

    cv::GaussianBlur(img, img, cv::Size(kernel_size, kernel_size), sigmaX);
    
    std::cout << "Showing blurred image... \nPress n to continue" << std::endl;
    cv::imshow("Image", img);
    while(char c = cv::waitKey(0) != 'n');

    return 0;
}