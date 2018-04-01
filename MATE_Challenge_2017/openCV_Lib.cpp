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
    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::WRITE);           // open the classifications file

    if (fsClassifications.isOpened() == false) {                                                        // if the file was not opened successfully
        std::cout << "error, unable to open training classifications file, exiting program\n\n";        // show error message
        return;                                                                                      // and exit program
    }

    fsClassifications << "classifications" << _matClassificationInts;        // write classifications into classifications section of classifications file
    fsClassifications.release();                                            // close the classifications file

    // save training images to file

    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::WRITE);         // open the training images file

    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
        return;                                                                              // and exit program
    }

    fsTrainingImages << "images" << _matTrainingImagesAsFlattenedFloats;         // write training images into images section of images file
    fsTrainingImages.release();                                                 // close the training images file
}

void static loadTrainingData(cv::Mat &_matClassificationInts, cv::Mat &_matTrainingImagesAsFlattenedFloats)
{
    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);        // open the classifications file

    if (fsClassifications.isOpened() == false) {                                                    // if the file was not opened successfully
        std::cout << "error, unable to open training classifications file, exiting program\n\n";    // show error message
        return;                                                                                  // and exit program
    }

    fsClassifications["classifications"] >> _matClassificationInts;      // read classifications section into Mat classifications variable
    fsClassifications.release();                                        // close the classifications file

    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);          // open the training images file

    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
        return;                                                                              // and exit program
    }

    fsTrainingImages["images"] >> _matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
    fsTrainingImages.release();                                                 // close the traning images file
}

void static processFrame (cv::Mat &imgSrc, std::vector<std::vector<cv::Point> > &_ptContours, std::vector<cv::Vec4i> &_v4iHierarchy) {
    cv::Mat imgGrayscale;               // 
    cv::Mat imgBlurred;                 // declare various images
    cv::Mat imgThresh;                  //
    cv::Mat imgThreshCopy;              //

    if (imgSrc.empty()) {                               // if unable to open image
        std::cout << "error: image not read\n\n";         // show error message on command line
        return;                                                  // and exit program
    }

    cv::cvtColor(imgSrc, imgGrayscale, CV_BGR2GRAY);        // convert to grayscale

    // input, output, smoothing window, blur factor
    cv::GaussianBlur(imgGrayscale, imgBlurred, cv::Size(5, 5), 0);

    // filter image from grayscale to black and white
    // input, output
    cv::adaptiveThreshold(imgBlurred, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);

    cv::imshow("imgThresh", imgThresh);         // show threshold image for reference

    imgThreshCopy = imgThresh.clone();          // make a copy of the thresh image, this in necessary b/c findContours modifies the image

    cv::findContours(imgThreshCopy, _ptContours, _v4iHierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}