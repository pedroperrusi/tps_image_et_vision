#include <iostream>
#include <opencv2/opencv.hpp>

/*
Global variables
*/
cv::Mat imgIn, imgGray, imgBlur, imgCanny, mouseImage;
std::string blurWindowName = "Blurred Image";
std::string edgesWindowName = "Edges Image";
std::string mouseWindowName = "Drawing Image";
// gaussian blur
const int maxSigmaX = 90;
int sigmaX = 1;
const int maxSigmaY = 90;
int sigmaY = 1;
const int maxKernelSize = 15;
int kernel_size = 1;
// canny
int const max_lowThreshold = 100;
int lowThreshold;
int ratio = 3;
// points cropping
std::vector<cv::Point> clicked_Points;

/**
 * @function trackbar callback for sigmaX
 */
static void onTrackbarSigmaX( int value, void* )
{
    sigmaX = value;
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);
    cv::imshow(blurWindowName, imgBlur);
}

/**
 * @function trackbar callback for sigmaY
 */
static void onTrackbarSigmaY( int value, void* )
{
    sigmaX = value;
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);
    cv::imshow(blurWindowName, imgBlur);
}

/**
 * @function trackbar callback for kernel
 */
static void onTrackbarKernel( int value, void* )
{
    kernel_size = 2*kernel_size+1;
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);
    cv::imshow(blurWindowName, imgBlur);
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
static void CannyThreshold(int, void*)
{
    cv::Mat detected_edges;
    /// Reduce noise with a kernel 3x3
    cv::blur( imgGray, detected_edges, cv::Size(3,3) );

    /// Canny detector
    cv::Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, 3 );

    /// Using Canny's output as a mask, we display our result
    imgCanny = cv::Scalar::all(0);

    imgIn.copyTo( imgCanny, detected_edges);
    cv::imshow( edgesWindowName, imgCanny );
 }

void fillPolygone(cv::Mat& img)
{
    const cv::Point* ppt[1] = {&clicked_Points[0]} ;
    int npt[] = {clicked_Points.size()};
      
    cv::fillPoly( 
              img,
              ppt,
              npt,
              1,
              cv::Scalar( 255, 255, 255 ));

}

/*
* @function on_mouse callback for mouse interface
*/
static void on_mouse(int event, int x, int y, int, void*)
{
    // if click droit, reset mouseImage to original image
    if(event == cv::EVENT_RBUTTONDOWN)
    {
        fillPolygone(mouseImage);
        clicked_Points.clear();
    }
    if(event == cv::EVENT_LBUTTONDOWN)
    {
        cv::Point pt(x, y);
        cv::circle(mouseImage, pt, 5, cv::Scalar(0,0,255));
        clicked_Points.push_back(pt);
    }
    cv::imshow(mouseWindowName, mouseImage);
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
    cv::createTrackbar( TrackbarName, blurWindowName, &sigmaX, maxSigmaX, onTrackbarSigmaX);
    // Sigma Y
    sprintf(TrackbarName, "SigmaY %d", maxSigmaY );
    cv::createTrackbar( TrackbarName, blurWindowName, &sigmaY, maxSigmaY, onTrackbarSigmaY);
    // Kernel Size
    sprintf(TrackbarName, "Kernel Size %d", maxKernelSize );
    cv::createTrackbar( TrackbarName, blurWindowName, &kernel_size, maxKernelSize, onTrackbarKernel);

    // Call first gaussian blur
    cv::GaussianBlur(imgIn, imgBlur, cv::Size(kernel_size, kernel_size), sigmaX, sigmaY);

    std::cout << "Showing blurred image... \nPress n to continue" << std::endl;
    cv::imshow(blurWindowName, imgBlur);
    while(char c = cv::waitKey(0) != 'n');

    /* Canny edges detection */
    // Convert image to grayscale
    cv::cvtColor( imgIn, imgGray, cv::COLOR_RGB2GRAY );
    /// Create a window
    cv::namedWindow( edgesWindowName, cv::WINDOW_AUTOSIZE );
    /// Create a Trackbar for user to enter threshold
    cv::createTrackbar( "Min Threshold:", edgesWindowName, &lowThreshold, max_lowThreshold, CannyThreshold );
    /// Show the image
    CannyThreshold(0, 0);

    std::cout << "Showing edges image... \nPress n to continue" << std::endl;
    while(char c = cv::waitKey(0) != 'n');

    /* Exploring Mouse Callback */
    std::cout << "Showing original image. May the drawing begin!" << std::endl;
    imgIn.copyTo(mouseImage);
    /// Create a window
    cv::namedWindow( mouseWindowName, cv::WINDOW_AUTOSIZE );
    cv::setMouseCallback( mouseWindowName, on_mouse, 0 );
    cv::imshow(mouseWindowName, mouseImage);

    while(char c = cv::waitKey(0) != 'n');

    return 0;
}