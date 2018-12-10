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
    
    return 0;
}