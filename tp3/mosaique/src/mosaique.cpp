#include <iostream>
#include <opencv2/opencv.hpp>

#define BIG_VALUE 100000000

/*
    On souhaite combiner les images de deux photographies proches
*/
void help()
{
    std::cout << "On souhaite creer un mosaïque a partir de deux images d'une même scene prise en deux points de vue differents.\n" 
              << "Entre les deux prises, la câmera a effectué une transition paralèle au plan de l'image."
    << std::endl;
}

void loopWaitKey(char key);

void myCornerHarris(cv::Mat src_gray, cv::Mat& dst);

std::vector<cv::Point> circleOverEdges(cv::Mat features, cv::Mat& drawing, int thresh);

std::vector<int> correspondFeatures(cv::Mat, std::vector<cv::Point>, cv::Mat, std::vector<cv::Point>, int w);

cv::Mat composeImages(cv::Mat img1, cv::Mat img2);

void drawCorrespondingLines(cv::Mat, cv::Mat, cv::Mat&, std::vector<cv::Point>, std::vector<cv::Point>, std::vector<int>);

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
    cv::cvtColor(img1, gray1, cv::COLOR_RGB2GRAY);
    cv::cvtColor(img2, gray2, cv::COLOR_RGB2GRAY);
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
    std::vector<cv::Point> C1 = circleOverEdges(features1, drawing1, thresh);
    cv::Mat drawing2; img2.copyTo(drawing2);
    std::vector<cv::Point> C2 = circleOverEdges(features2, drawing2, thresh);
    // Visualization
    cv::imshow("Features over image T1", drawing1);
    cv::imshow("Features over image T2", drawing2);
    loopWaitKey('n');

    /* Mise en correspondence */
    // get correspondence of C1 to C2
    std::vector<int> C1_2 = correspondFeatures(gray1, C1, gray2, C2, 7);

    // Draw corresponding lines on a new image
    cv::Mat mosaique = composeImages(img1, img2);
    cv::imshow("Mosaique", mosaique);
    loopWaitKey('n');

    drawCorrespondingLines(img1,  img2, mosaique, C1, C2, C1_2);
    cv::imshow("Mosaique", mosaique);
    loopWaitKey('n');

    // get correspondence C2 C1
    std::vector<int> C2_1 = correspondFeatures(gray2, C2, gray1, C1, 7);

    // simetric  correspondence
    std::vector<cv::Point> CSimetric;
    // find corresponding indexes
    for(size_t i = 0; i < C1_2.size(); i++)
    {
        for(size_t j = 0; j < C2_1.size(); j++)
        {
            if( C1[C2_1[i]] == C2[C1_2[j]] )
            {
                CSimetric.push_back(C1[C2_1[i]]);
            }
        }
    }

    return 0;
}


// Wait for user input on image window until he presses the key requested
void loopWaitKey(char key)
{
    std::cout << "Press " << key << " to continue" << std::endl;
    while(char c = cv::waitKey(0) != key);
}

cv::Mat composeImages(cv::Mat img1, cv::Mat img2)
{
    // set height as the maximum value between img1 et img2
    int dstHeight = img1.rows > img2.rows ? img1.rows : img2.rows;
    // set width as the sum of widiths
    int dstWidth = img1.cols + img2.cols;
    // create blank image
    cv::Mat dst = cv::Mat(dstHeight, dstWidth, img1.type(), cv::Scalar(0,0,0)); 
    // define region of interest for image 1 and copy it
    cv::Mat targetROI = dst(cv::Rect(0,0,img1.cols, img1.rows));
    img1.copyTo(targetROI);
    // define region of interest for image 2 and copy it
    targetROI = dst(cv::Rect(img1.cols,0,img2.cols, img2.rows));
    img2.copyTo(targetROI);
    // return composed image
    return dst;
}

void drawCorrespondingLines(cv::Mat img1, cv::Mat img2, cv::Mat& dst, std::vector<cv::Point> C1, std::vector<cv::Point> C2,  std::vector<int> C1_2)
{
    for(size_t i = 0; i < C1.size(); i++)
    {
        cv::Point deplacedC1_2 = cv::Point(C2[C1_2[i]].x + img1.cols, C2[C1_2[i]].y);
        cv::line(dst, C1[i], deplacedC1_2, cv::Scalar(255,255,255));
    }
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

// Brute force correponding features
// Return vector of corresponding pixels of gray1 and gray2 based on C1 and C2 inside window w
std::vector<int> correspondFeatures(cv::Mat gray1, std::vector<cv::Point> C1, 
                                          cv::Mat gray2, std::vector<cv::Point> C2,
                                          int w=7)
{
    // generate output vector initialized to -1
    std::vector<int> correspondance12(C1.size(), -1);
    // subimages for each window
    cv::Mat windowC1, windowC2;
    for(size_t i_C1 = 0; i_C1 < C1.size(); i_C1++)
    {
        int min_idx = -1; // invalid index
        double minDiff = BIG_VALUE; // arbitrary big value
        for(size_t j_C2 = 0; j_C2 < C2.size(); j_C2++)
        {
            // Crate subimages of widith w centered in C1[i] - w and C2[j] - w
            // they are rectangles of dimension 2*w.
            windowC1 = gray1(cv::Rect(C1[i_C1].x - w, C1[i_C1].y - w, 2*w, 2*w));
            windowC2 = gray2(cv::Rect(C2[j_C2].x - w, C2[j_C2].y - w, 2*w, 2*w));
            // compute their difference ... (http://answers.opencv.org/question/104818/squaring-elements-of-a-matrix-and-summing-them/)
            cv::Mat diffWindow = windowC1 - windowC2;
            // elementwise multiplication
            cv::Mat squareDiff = diffWindow.mul(diffWindow);
            // computing its sum
            double sumDiff = cv::sum(squareDiff)[0];
            if(sumDiff < minDiff)
            {
                minDiff = sumDiff;
                min_idx = j_C2;
            }
        }
        // if there is a valid index, a minimum is been found
        if(min_idx != -1)
        {
            correspondance12[i_C1] = min_idx;
        }
    }
    for (const auto& i: correspondance12)
        std::cout << C2[i] << ' ' << std::endl;

    return correspondance12;
}