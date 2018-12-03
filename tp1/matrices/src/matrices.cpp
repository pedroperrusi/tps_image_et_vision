#include <stdio.h>
#include <opencv2/opencv.hpp>

/*
    Exercise 3: Manipulation des matrices
*/
int main( int argc, char** argv )
{
    std::cout << "Starting matrices program..." << std::endl; 
    // Create matrix 2x2 floating point 32 bits, single channel.
    // other possible types are : 
    //      CV_8UFC1 (8 bits unsigned integer single channel)
    //      CV_32FC3 (32 bists floating point 3 channels)
    cv::Mat R(2,2,CV_32FC1);
    // iterate over R to fulfill the values (https://www.mathsisfun.com/algebra/matrix-inverse.html)
    // R = [-3 6; 2 -5]
    for(size_t i = 0; i < R.rows; i++)
    {
        for(size_t j = 0; j < R.cols; j++)
        {
            std::cout << "Insert the element (" << i << "," << j << ") of the matrix" << std::endl; 
            std::cin >> R.at<float>(i,j);
        }
    }
    // Print out matrix (https://stackoverflow.com/questions/7970988/print-out-the-values-of-a-mat-matrix-in-opencv-c)
    std::cout << "\tInitial matrix:" << std::endl 
        << "R = "<< std::endl  
        << " "  << R << std::endl;

    std::cout << "\tInverse of matrix:" << std::endl 
        << "R.inv() = "<< std::endl 
        << " "  << R.inv() << std::endl;

    // multiply the matrix by points given by the user
    cv::Mat P(2,1,CV_32FC1);
    
    std::cout << "Give a coordinate X for the point" << std::endl; 
    std::cin >> P.at<float>(0,0);
    std::cout << "Give a coordinate Y for the point" << std::endl;
    std::cin >> P.at<float>(1,0);
    
    std::cout <<  "Matrice R: \n" << R << "\nMultiplie par P:\n" << P << std::endl;
    std::cout << "Results: \n" << R*P << std::endl; 
    
    return 0;
}