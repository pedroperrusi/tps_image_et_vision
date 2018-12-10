#include <iostream>
#include <opencv2/opencv.hpp>

/*
    On souhaite compter automatiquement le nombre de coins sur l'image.
    Tout le development a été basée sur https://docs.opencv.org/3.1.0/d2/dbd/tutorial_distance_transform.html
*/
void help()
{
    std::cout << "On souhaite creer un mosaïque a partir de deux images d'une même scene prise en deux points de vue differents.\n" 
              << "Entre les deux prises, la câmera a effectué une transition paralèle au plan de l'image."
    << std::endl;
}

// Wait for user input on image window until he presses the key requested
void loopWaitKey(char key)
{
    std::cout << "Press " << key << " to continue" << std::endl;
    while(char c = cv::waitKey(0) != key);
}

// Perform corner harris detection accordingly to TP instructions
// outpur image dst is normalized and may change its type
void myCornerHarris(cv::Mat src_gray, cv::Mat& dst)
{
    int block_size = 2;
    int aperture_size = 3;
    double k = 0.04;

    cv::cornerHarris( src_gray, dst, block_size, aperture_size, k );

    // Normalize and scale results
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    //cv::convertScaleAbs(dst, dst); // important if we want to visualize, but breaks the threshold
}

// Draws circle over feature image points which are above thresh
// Returns a vector of points corresponding to the drawn points
std::vector<cv::Point> circleOverEdges(cv::Mat features, cv::Mat& drawing, int thresh)
{
    std::vector<cv::Point> vecPoints;
    for( int i = 0; i < features.rows ; i++ )
    {
        for( int j = 0; j < features.cols; j++ )
        {
            if( (int) features.at<float>(i,j) > thresh )
            {
                cv::circle( drawing, cv::Point(j,i), 5,  cv::Scalar(255,255,255), 2, 8, 0 );
                vecPoints.push_back(cv::Point(j,i));
            }
        }
    }
    return vecPoints;
}

int main()
{
    help();

    // reading both images 
    cv::Mat img1 = cv::imread( "data/t1.png" );
    cv::Mat img2 = cv::imread( "data/t2.png" );
    // Visualisation
    cv::imshow("Image T1", img1);
    cv::imshow("Image T2", img2);
    loopWaitKey('n');
    // convert to grayscale
    cv::Mat gray1, gray2;
    cv::cvtColor(img1, gray1, CV_BGR2GRAY);
    cv::cvtColor(img2, gray2, CV_BGR2GRAY);
    // Visualization
    cv::imshow("Image T1", gray1);
    cv::imshow("Image T2", gray2);
    loopWaitKey('n');

    /* Call Harris Corner Detection */
    cv::Mat features1 =  cv::Mat::zeros( gray1.size(), CV_32FC1 );
    myCornerHarris(gray1, features1);
    cv::Mat features2 =  cv::Mat::zeros( gray2.size(), CV_32FC1 );
    myCornerHarris(gray2, features2);
    // draw circles over the points over thresh = 125
    int thresh = 125;
    cv::Mat drawing1; img1.copyTo(drawing1);
    std::vector<cv::Point> C1 =  circleOverEdges(features1, drawing1, thresh);
    cv::Mat drawing2; img2.copyTo(drawing2);
    std::vector<cv::Point> C2 = circleOverEdges(features2, drawing2, thresh);
    // Visualization
    cv::imshow("Features over image T1", drawing1);
    cv::imshow("Features over image T2", drawing2);
    loopWaitKey('n');
    


    return 0;
}