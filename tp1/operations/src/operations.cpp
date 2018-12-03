#include <iostream>
#include <opencv2/opencv.hpp>

/*
Global variables
*/
cv::Mat imgIn, imgBlur;
std::string blurWindowName = "Blurred Image";
const int maxSigmaX = 90;
int sigmaX = 1;
const int maxSigmaY = 90;
int sigmaY = 1;
const int maxKernelSize = 15;
int kernel_size = 1;


/**
 * @function trackbar callback for sigma
 */
void onTrackbarSigmaX( int value, void* )
{
    sigmaX = value;
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);
    cv::imshow(blurWindowName, imgBlur);
}

/**
 * @function trackbar callback for sigma
 */
void onTrackbarSigmaY( int value, void* )
{
    sigmaX = value;
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);
    cv::imshow(blurWindowName, imgBlur);
}

/**
 * @function trackbar callback for kernel
 */
void onTrackbarKernel( int value, void* )
{
    kernel_size = 2*kernel_size+1;
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);
    cv::imshow(blurWindowName, imgBlur);
}

/*
    Operations des Images
*/
int main()
{
    // Load image of lena
    imgIn = cv::imread( "data/lena.jpg" );
    if( !imgIn.data )
    {
        printf("No image data\n");
        return -1;
    }
    // Show image
    std::cout << "Showing original image... \nPress n to continue" << std::endl;

    cv::imshow("Image", imgIn);
    while(char c = cv::waitKey(0) != 'n');


    // // Apply gaussian blur
    // std::cout << "Applying gaussian blur... \n input kernel size (recommends odd values):\n-> ";
    // std::cin >> kernel_size;
    // std::cout << "input sigmaX:\n-> ";
    // std::cin >> sigmaX;

    /* Gaussian Blur */
    // Create trackbars
    cv::namedWindow(blurWindowName, cv::WINDOW_AUTOSIZE); // Create Window
    char TrackbarName[50];
    // Sigma X
    sprintf(TrackbarName, "SigmaX %d", maxSigmaX );
    cv::createTrackbar( "SigmaX Slider", blurWindowName, &sigmaX, maxSigmaX, onTrackbarSigmaX);
    // Sigma Y
    sprintf(TrackbarName, "SigmaY %d", maxSigmaY );
    cv::createTrackbar( "SigmaY Slider", blurWindowName, &sigmaY, maxSigmaY, onTrackbarSigmaY);
    // Kernel Size
    sprintf(TrackbarName, "Kernel Size %d", maxKernelSize );
    cv::createTrackbar( "Kernel Slider", blurWindowName, &kernel_size, maxKernelSize, onTrackbarKernel);

    // Call first gaussian blur
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);

    std::cout << "Showing blurred image... \nPress n to continue" << std::endl;
    cv::imshow(blurWindowName, imgBlur);
    while(char c = cv::waitKey(0) != 'n');

    /* Canny edges detection */

    return 0;
}