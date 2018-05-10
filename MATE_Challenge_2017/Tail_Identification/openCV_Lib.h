#include <iostream> // for standard I/O
#include <string>   // for strings
#include <vector>

#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/ml/ml.hpp>

// global variables
const int MIN_CONTOUR_AREA = 100;
const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;

void static uploadTrainingData (cv::Mat _matClassificationInts, cv::Mat _matTrainingImagesAsFlattenedFloats) {
    // Store Classification Mapping part
    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::WRITE);
    if (fsClassifications.isOpened() == false) {
        std::cout << "error, unable to open training classifications file, exiting program\n\n";
        return;
    }
    fsClassifications << "classifications" << _matClassificationInts;
    fsClassifications.release();

    // Store Image Mapping part
    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::WRITE);
    if (fsTrainingImages.isOpened() == false) {
        std::cout << "error, unable to open training images file, exiting program\n\n";
        return;
    }
    fsTrainingImages << "images" << _matTrainingImagesAsFlattenedFloats;
    fsTrainingImages.release();
}

void static loadTrainingData (cv::Mat &_matClassificationInts, cv::Mat &_matTrainingImagesAsFlattenedFloats)
{
    // Load in Classifications Mapping part
    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);
    if (fsClassifications.isOpened() == false) {
        std::cout << "error, unable to open training classifications file, exiting program\n\n";
        return;
    }
    fsClassifications["classifications"] >> _matClassificationInts;
    fsClassifications.release();

    // Load in Image Mapping part
    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);
    if (fsTrainingImages.isOpened() == false) {
        std::cout << "error, unable to open training images file, exiting program\n\n";
        return;
    }
    fsTrainingImages["images"] >> _matTrainingImagesAsFlattenedFloats;
    fsTrainingImages.release();
}

cv::Mat static processFrame (cv::Mat &imgSrc, std::vector<std::vector<cv::Point> > &_ptContours, std::vector<cv::Vec4i> &_v4iHierarchy) {
    cv::Mat imgGrayscale, imgBlurred, imgThresh, imgThreshCopy;

    if (imgSrc.empty()) {
        std::cout << "error: image not read\n\n";
        std::exit(-1);
    }
    cv::imshow("source", imgSrc);
    // convert to grayscale
    cv::cvtColor(imgSrc, imgGrayscale, CV_BGR2GRAY);
    // blurring
    cv::GaussianBlur(imgGrayscale, imgBlurred, cv::Size(5, 5), 0);
    // filter image from grayscale to black and white
    cv::adaptiveThreshold(imgBlurred, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
    // show threshold image for reference
    cv::imshow("imgThresh", imgThresh);
    // make a copy of the thresh image, this in necessary b/c findContours modifies the image
    imgThreshCopy = imgThresh.clone();
    // loads _ptContours with contours cut out from the imgThreshCopy
    // https://stackoverflow.com/questions/44601734/cv2-findcontours-not-able-to-detect-contours
    cv::findContours(imgThreshCopy, _ptContours, _v4iHierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE); // RETR_EXTERNAL to RETR_LIST
    cv::waitKey();
    return imgThresh;
}