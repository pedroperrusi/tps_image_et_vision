#include <stdio.h>
#include <iostream>
#include <vector>
#include <numeric> 
#include <opencv2/opencv.hpp>

void drawCrossC1(cv::Mat grayImg, int thickness=2)
{
    int imgWidth = grayImg.cols;
    int imgHeight = grayImg.rows;
    // Middle points for drawing a cross
    int midH = floor(imgWidth / 2);
    int midW = floor(imgHeight / 2);

    // Draw horizontal line
    cv::line(grayImg, cv::Point(0,midH), cv::Point(imgWidth,midH),cv::Scalar(255),thickness);
    // Draw vertical line
    cv::line(grayImg, cv::Point(midW,0), cv::Point(midW,imgHeight),cv::Scalar(255), thickness);

    return;
}

void drawCrossC3(cv::Mat grayImg, int thickness=2)
{
    int imgWidth = grayImg.cols;
    int imgHeight = grayImg.rows;
    // Middle points for drawing a cross
    int midH = floor(imgWidth / 2);
    int midW = floor(imgHeight / 2);

    // Draw horizontal line
    cv::line(grayImg, cv::Point(0,midH), cv::Point(imgWidth,midH),cv::Scalar(0,0,255),thickness);
    // Draw vertical line
    cv::line(grayImg, cv::Point(midW,0), cv::Point(midW,imgHeight),cv::Scalar(0,0,255), thickness);

    return;
}

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

    // Draw white cross over image
    drawCrossC1(grey);
    cv::imshow( "Grey Image", grey);
    cv::waitKey(0);

    std::vector<int> pointsX;
    std::vector<int> pointsY;
    // reads four points, shows them and compute baricentre
    std::cout << "Well take four points and draw them for you" << std::endl;
    for(size_t i = 0; i < 4; i++)
    {
        int X, Y;
        std::cout << "Input " << i << " point X:" << std::endl;
        std::cin >> X;
        std::cout << "Input " << i << " point Y:" << std::endl;
        std::cin >> Y;
        // Draw them on image
        cv::circle(img, cv::Point(X, Y), 5 , cv::Scalar(0,255,0), 2);
        pointsX.push_back(X);
        pointsY.push_back(Y);
    }
    std::cout << "Understood, look at the image!" << std::endl;

    cv::imshow("Image", img);
    cv::waitKey(0);

    std::cout << "Computing barycentre..." << std::endl;

    float barX = std::accumulate(pointsX.begin(), pointsX.end(), 0.0)/pointsX.size();
    float barY = std::accumulate(pointsY.begin(), pointsY.end(), 0.0)/pointsY.size();

    std::cout << "Barycentre: " << barX << ", " << barY << std::endl;
    cv::circle(img, cv::Point2f(barX, barY), 5 , cv::Scalar(0,0,255), 2);

    cv::imshow("Image", img);
    cv::waitKey(0);

    // Draws a colored cross on image
    drawCrossC3(img);
    cv::imshow("Image", img);
    cv::waitKey(0);

    return 0;
}