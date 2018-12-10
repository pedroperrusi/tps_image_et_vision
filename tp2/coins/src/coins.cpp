#include <iostream>
#include <opencv2/opencv.hpp>

/*
    On souhaite compter automatiquement le nombre de coins sur l'image.
    Tout le development a été basée sur https://docs.opencv.org/3.1.0/d2/dbd/tutorial_distance_transform.html
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

    // Q7: Utiliser find contours pour trouver des contours sur la carte de distance
    std::cout << "Finding image contours... \nPress q to continue" << std::endl;
    // s'assurer que le type de l'image est CV_8U
    imgDistance.convertTo(imgDistance, CV_8U);
    // create instances to recieve contour information
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    // find contours
    cv::findContours(imgDistance, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE );
    std::cout << "Number of contours found: " << contours.size() << std::endl;
    
    /*Locate coins on image*/
    cv::Mat markers = cv::Mat::zeros( imgDistance.size(), CV_32FC1 );
    // Background marker located at (5,5)
    cv::circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
    // Draw each countour
    for( int i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar::all(static_cast<int>(i)+1);
        cv::drawContours(markers, contours, i, color, -1, 8, hierarchy);
    }
    cv::imshow("Contours Image", 1000 * markers);
    while(char c = cv::waitKey(0) != 'q');

    // /*Find contours centres*/
    // /// Get the moments
    // std::vector<cv::Moments> mu(contours.size() );
    // for( int i = 0; i < contours.size(); i++ )
    //     { mu[i] = cv::moments( contours[i], false ); }

    // ///  Get the mass centers and draw them over a copy of the image:
    // cv::Mat imgDrawing;
    // img.copyTo(imgDrawing);
    // std::vector<cv::Point2f> mc( contours.size() );
    // for( int i = 0; i < contours.size(); i++ )
    // { 
    //     mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    //     cv::circle(imgDrawing, mc[i], 3, cv::Scalar::all(255));
    // }

    // cv::imshow("Seeds over Image", imgDrawing);
    // while(char c = cv::waitKey(0) != 'q');

    /* Apply Watershed algorithm pour segmenter coins.jpg */
    // attention to image types
    cv::Mat img_8UC3;
    img.convertTo(img_8UC3, CV_8UC3);
    markers.convertTo(markers, CV_32SC1);
    cv::watershed(img_8UC3, markers);
    cv::Mat marker_aux = cv::Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(marker_aux, CV_8UC1);
    cv::bitwise_not(marker_aux, marker_aux);
    cv::imshow("Preparation to Watershed", marker_aux);
    while(char c = cv::waitKey(0) != 'q');

    // Generate random colors
    std::vector<cv::Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = cv::theRNG().uniform(0, 255);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);
        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the segmented image
    cv::Mat segmentedImage = cv::Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            // rappel: index = 0 est le background
            if (index > 0 && index <= static_cast<int>(contours.size()))
                segmentedImage.at<cv::Vec3b>(i,j) = colors[index-1];
            else
                segmentedImage.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
        }
    }
    // Visualize the final image
    imshow("Final Result", segmentedImage);
    while(char c = cv::waitKey(0) != 'q');

    return 0;
}