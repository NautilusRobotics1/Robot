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
	cv::addWeighted(imgGrayscale, 1.5, imgBlurred, -0.5, 0, imgBlurred);
	cv::imshow("imgSharpened", imgBlurred);
	
	cv::findContours(imgBlurred.clone(), _ptContours, _v4iHierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE); // RETR_EXTERNAL to RETR_LIST																										  // Fill holes in each contour
	cv::drawContours(imgBlurred, _ptContours, -1, CV_RGB(255, 255, 255), -1);
	cv::imshow("found contours", imgBlurred);
	/*
	// Set up the parameters (check the defaults in opencv's code in blobdetector.cpp)
	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = 0.0f;//50.0f;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = true;
	params.minArea = 20.0f;
	params.maxArea = 1000000000000000000000.0f;
	//params.maxArea = 500.0f; // Able to detect just the characters
	// ... any other params you don't want default value

	// set up and create the detector using the parameters
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat im_with_keypoints;

	// Detecting Blobs
	detector->detect(imgGrayscale, keypoints);
	cv::drawKeypoints(imgSrc, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("keypoints", im_with_keypoints);
	*/
	cv::waitKey(0);
/*
  // Clear for Blue triangle, but now there is a triangle from the table too
  int low_r = 0, low_g = 0, low_b = 0;
  int high_r = 80, high_g = 80, high_b = 255;
  // blurring
  cv::GaussianBlur(imgSrc, imgBlurred, cv::Size(5, 5), 0);
  cv::imshow("imgBlurred", imgBlurred);

  // https://docs.opencv.org/3.4.0/d7/d4d/tutorial_py_thresholding.html
  // https://stackoverflow.com/questions/12204522/efficiently-threshold-red-using-hsv-in-opencv
  //cv::cvtColor(imgSrc, imgRGB, CV_RGB2HSV); // Isn't really grayscale though
  cv::inRange(imgBlurred, cv::Scalar(low_b, low_g, low_r), cv::Scalar(high_b, high_g, high_r), imgThresh);
  cv::imshow("imgThresh", imgThresh);

  imgThreshCopy = imgThresh.clone();
  cv::findContours(imgThreshCopy, _ptContours, _v4iHierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE); // RETR_EXTERNAL to RETR_LIST
*/
  /*
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
  */
    return imgThresh;
}