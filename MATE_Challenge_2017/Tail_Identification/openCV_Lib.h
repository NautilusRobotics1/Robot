#include <iostream> // for standard I/O
#include <string>   // for strings
#include <vector>

#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/ml/ml.hpp>
#include <opencv2\features2d.hpp> // For simple blob detection

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
	// convert to grayscale
	cv::cvtColor(imgSrc, imgGrayscale, CV_BGR2GRAY);
	// Blurring
	cv::GaussianBlur(imgGrayscale, imgBlurred, cv::Size(0, 0), 3);
	cv::imshow("imgBlurred", imgBlurred);
	// Sharpening
	//cv::addWeighted(imgGrayscale, 1.5, imgBlurred, -0.5, 0, imgBlurred); // Given parameters
	cv::addWeighted(imgGrayscale, 2, imgBlurred, -1.5, 0, imgBlurred);
	cv::imshow("imgSharpened", imgBlurred);
	
	cv::findContours(imgBlurred.clone(), _ptContours, _v4iHierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE); // RETR_EXTERNAL to RETR_LIST																										  // Fill holes in each contour
	cv::drawContours(imgBlurred, _ptContours, -1, CV_RGB(255, 255, 255), -1);
	cv::imshow("found contours", imgBlurred);

	// TODO: Connect Contours
	// https://stackoverflow.com/questions/19123165/join-close-enough-contours-in-opencv
	// https://docs.opencv.org/2.4/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html

	cv::waitKey(0);
	return imgThresh;
}