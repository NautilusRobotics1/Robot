#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

// global variables ///////////////////////////////////////////////////////////////////////////////
const int MIN_CONTOUR_AREA = 100;

const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;

///////////////////////////////////////////////////////////////////////////////////////////////////
class ContourWithData {
public:
    // member variables ///////////////////////////////////////////////////////////////////////////
    std::vector<cv::Point> ptContour;           // contour
    cv::Rect boundingRect;                      // bounding rect for contour
    float fltArea;                              // area of contour

    ///////////////////////////////////////////////////////////////////////////////////////////////
    bool checkIfContourIsValid() {                              // obviously in a production grade program
        if (fltArea < MIN_CONTOUR_AREA) return false;           // we would have a much more robust function for 
        return true;                                            // identifying if a contour is valid !!
    }

    // this function allows us to sort
    static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {
        return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);  // the contours from left to right
    }

};

void loadTrainingData(cv:Mat &_matClassificationInts, cv:Mat &_matTrainingImagesAsFlattenedFloats);
void processFrame(std::vector<ContourWithData> &_allContoursWithData, std::vector<ContourWithData> &_validContoursWithData);
void processContours(cv::Ptr<cv::ml::KNearest> &_kNearest, std::vector<ContourWithData> &_validContoursWithData, std::string &_strFinalString);

int main(int argc, char *argv[])
{
    /*******************Init Video Feed****************************/
    int frameNum = -1;          // Frame counter

    VideoCapture captRefrnc(0); // Using the default camera
    if (!captRefrnc.isOpened())
    {
        std::cout  << "Could not open reference " << sourceReference << std::endl;
        return -1;
    }

    const char* WIN_RF = "Reference";
    /*******************Init Video Feed****************************/

    /*******************Loading Train Data*************************/
    cv::Mat matClassificationInts;      // we will read the classification numbers into this variable as though it is a vector
    cv::Mat matTrainingImagesAsFlattenedFloats;         // we will read multiple images into this single image variable as though it is a vector
    loadTrainingData(matClassificationInts,matTrainingImagesAsFlattenedFloats);
    /*******************Loading Train Data*************************/

    /*******************Train KNN**********************************/
    cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());            // instantiate the KNN object
    kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);
    /*******************Train KNN**********************************/

    /*******************Prep for Processing************************/
    std::vector<ContourWithData> allContoursWithData;           // declare empty vectors,
    std::vector<ContourWithData> validContoursWithData;         // we will fill these shortly
    /*******************Prep for Processing************************/

    Mat frameReference;

    for(;;) // takes a frame and process it for colors/shapes/alphanumerics
    {
        captRefrnc >> frameReference;
        if (frameReference.empty())
        {
            std::cout << " < < <  Game over!  > > > ";
            break;
        }
        ++frameNum;
        std::cout << "Frame: " << frameNum << "# ";

        // Using KNN for alphanumeric/color/shape recognition
        /*******************Processing Frame***************************/
        processFrame(allContoursWithData,validContoursWithData);
        std::string strFinalString;  // declare final string, this will have the final number sequence by the end of the program
        /*******************Processing Contours************************/
        processContours(kNearest,validContoursWithData,strFinalString);
        /*******************Processing Contours************************/
        /*******************Processing Frame***************************/

        std::cout << std::endl;
      /*
        std::std::cout << "\n\n" << "numbers read = " << strFinalString << "\n\n";       // show the full string
        cv::imshow("matTestingNumbers", matTestingNumbers);     // show input image with green boxes drawn around found digits
      */
        imshow(WIN_RF, frameReference);
        char c = (char)waitKey(delay);
        if (c == 27) break;
    }
    return 0;
}

void loadTrainingData(cv:Mat &_matClassificationInts, cv:Mat &_matTrainingImagesAsFlattenedFloats)
{
    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);        // open the classifications file

    if (fsClassifications.isOpened() == false) {                                                    // if the file was not opened successfully
        std::std::cout << "error, unable to open training classifications file, exiting program\n\n";    // show error message
        return(0);                                                                                  // and exit program
    }

    fsClassifications["classifications"] >> _matClassificationInts;      // read classifications section into Mat classifications variable
    fsClassifications.release();                                        // close the classifications file

    // read in training images ////////////////////////////////////////////////////////////


    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);          // open the training images file

    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        std::std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
        return(0);                                                                              // and exit program
    }

    fsTrainingImages["images"] >> _matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
    fsTrainingImages.release();                                                 // close the traning images file
}

void processFrame (std::vector<ContourWithData> &_allContoursWithData, std::vector<ContourWithData> &_validContoursWithData)
{
    cv::Mat matGrayscale;           //
    cv::Mat matBlurred;             // declare more image variables
    cv::Mat matThresh;              //
    cv::Mat matThreshCopy;          //

    cv::cvtColor(matTestingNumbers, matGrayscale, CV_BGR2GRAY);         // convert to grayscale

                                                                        // blur
    cv::GaussianBlur(matGrayscale,              // input image
        matBlurred,                // output image
        cv::Size(5, 5),            // smoothing window width and height in pixels
        0);                        // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value

                                // filter image from grayscale to black and white
    cv::adaptiveThreshold(matBlurred,                           // input image
        matThresh,                            // output image
        255,                                  // make pixels that pass the threshold full white
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,       // use gaussian rather than mean, seems to give better results
        cv::THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
        11,                                   // size of a pixel neighborhood used to calculate threshold value
        2);                                   // constant subtracted from the mean or weighted mean

    matThreshCopy = matThresh.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image

    std::vector<std::vector<cv::Point> > ptContours;        // declare a vector for the contours
    std::vector<cv::Vec4i> v4iHierarchy;                    // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

    cv::findContours(matThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
        ptContours,                             // output contours
        v4iHierarchy,                           // output hierarchy
        cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
        cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

    for (int i = 0; i < ptContours.size(); i++) {               // for each contour
        ContourWithData contourWithData;                                                    // instantiate a contour with data object
        contourWithData.ptContour = ptContours[i];                                          // assign contour to contour with data
        contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);         // get the bounding rect
        contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);               // calculate the contour area
        _allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
    }

    for (int i = 0; i < allContoursWithData.size(); i++) {                      // for all contours
        if (allContoursWithData[i].checkIfContourIsValid()) {                   // check if valid
            _validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
        }
    }
    // sort contours from left to right
    std::sort(_validContoursWithData.begin(), _validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);
}

void processContours(cv::Ptr<cv::ml::KNearest> &_kNearest, std::vector<ContourWithData> &_validContoursWithData, std::string &_strFinalString)
{
    // for each contour
    for (int i = 0; i < _validContoursWithData.size(); i++) {

        // draw a green rect around the current char
        cv::rectangle(matTestingNumbers,  // draw rectangle on original image
            _validContoursWithData[i].boundingRect,  // rect to draw
            cv::Scalar(0, 255, 0),  // green
            2);  // thickness

        cv::Mat matROI = matThresh(_validContoursWithData[i].boundingRect);  // get ROI image of bounding rect

        cv::Mat matROIResized;
        cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));  // resize image, this will be more consistent for recognition and storage

        cv::Mat matROIFloat;
        matROIResized.convertTo(matROIFloat, CV_32FC1);  // convert Mat to float, necessary for call to find_nearest

        cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

        cv::Mat matCurrentChar(0, 0, CV_32F);

        kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!

        float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

        _strFinalString = _strFinalString + char(int(fltCurrentChar));        // append current char to full string
    }
}