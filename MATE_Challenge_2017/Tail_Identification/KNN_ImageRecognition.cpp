// KNN_ImageRecognition.cpp

#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <opencv2/videoio.hpp>

#include "openCV_Lib.h"

class ContourWithData {
    public:
        // member variables
        std::vector<cv::Point> ptContour;           // contour
        cv::Rect boundingRect;                      // bounding rect for contour
        float fltArea;                              // area of contour

        bool checkIfContourIsValid() {
            if (fltArea < MIN_CONTOUR_AREA) return false;
            return true;
        }

        // this function allows us to sort
        static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {
            return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);  // the contours from left to right
        }
};

void processContours(cv::Ptr<cv::ml::KNearest> &_kNearest, std::vector<ContourWithData> &_validContoursWithData, cv::Mat imgSrc, cv::Mat matThresh, std::string &_strFinalString);

int main(int argc, char *argv[])
{
    /*******************Init Video Feed****************************/
    cv::VideoCapture captRefrnc(0); // Using the default camera
    if (!captRefrnc.isOpened())
    {
        std::cout  << "Could not open reference vide" << std::endl;
        return -1;
    }
    const char* WIN_RF = "Reference";

    /*******************Loading Train Data*************************/
    cv::Mat matClassificationInts; // read in classifications
    cv::Mat matTrainingImagesAsFlattenedFloats; // read in matching images(floats)
    loadTrainingData(matClassificationInts,matTrainingImagesAsFlattenedFloats);

    /*******************Train KNN**********************************/
    cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());
    kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

    /*******************Prep for Processing************************/
    std::vector<ContourWithData> allContoursWithData;
    std::vector<ContourWithData> validContoursWithData;
    std::vector<std::vector<cv::Point> > ptContours;        // declare contours vector
    std::vector<cv::Vec4i> v4iHierarchy;                    // declare contours hierarchy
    cv::Mat frameReference;

    /*******************Processing Frame***************************/
    // takes a frame and process it for colors/shapes/alphanumerics
    for(;;)
    {
        captRefrnc >> frameReference;
        cv::Mat imgThresh = processFrame(frameReference, ptContours, v4iHierarchy);

        // Initialize allContoursWithData w/ boundingRect area
        for (int i = 0; i < ptContours.size(); i++) {
            ContourWithData contourWithData;
            contourWithData.ptContour = ptContours[i];
            contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
            contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);
            allContoursWithData.push_back(contourWithData);
        }

        // Filter out invalid contours
        for (int i = 0; i < allContoursWithData.size(); i++) {
            if (allContoursWithData[i].checkIfContourIsValid()) {
                validContoursWithData.push_back(allContoursWithData[i]);
            }
        }
        // sort contours from left to right
        std::sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);

        // End output, label
        std::string strFinalString;
        processContours(kNearest,validContoursWithData,frameReference,imgThresh,strFinalString);

        // show final string & input image with green boxes drawn around found digits
        std::cout << "\n\n" << "numbers read = " << strFinalString << "\n\n";
        imshow(WIN_RF, frameReference); 
        char c = (char)cv::waitKey(20);
        if (c == 27) break;
    }
    return 0;
}

void processContours(cv::Ptr<cv::ml::KNearest> &_kNearest, std::vector<ContourWithData> &_validContoursWithData, cv::Mat imgSrc, cv::Mat matThresh, std::string &_strFinalString)
{
    for (int i = 0; i < _validContoursWithData.size(); i++) {
        // draw a green rect around the current char atop original image
        cv::rectangle(imgSrc, _validContoursWithData[i].boundingRect, cv::Scalar(0, 255, 0), 2);

        // get ROI image of bounding rect
        cv::Mat matROI = matThresh(_validContoursWithData[i].boundingRect);

        // resize image, this will be more consistent for recognition and storage
        cv::Mat matROIResized;
        cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

        // convert Mat to float, necessary for call to find_nearest
        cv::Mat matROIFloat;
        matROIResized.convertTo(matROIFloat, CV_32FC1);  
        cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

        // Use KNN to determine the right classification for contour
        cv::Mat matCurrentChar(0, 0, CV_32F);
        _kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);
        float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

        // append current char to full string
        _strFinalString = _strFinalString + char(int(fltCurrentChar));
    }
}

