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
        double fltArea;                             // area of contour
        //cv::Moments moment;                         // center of contour

        // TODO: Possibly make more robust
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
    //cv::VideoCapture captRefrnc(0); // Using the default camera
    //if (!captRefrnc.isOpened())
    //{
    //    std::cout  << "Could not open reference vide" << std::endl;
    //    return -1;
    //}
    const char* WIN_RF = "Reference";

    /*******************Loading Train Data*************************/
    cv::Mat matClassificationInts; // read in classifications
    cv::Mat matTrainingImagesAsFlattenedFloats; // read in matching images(floats)
    loadTrainingData(matClassificationInts,matTrainingImagesAsFlattenedFloats);

    /*******************Train KNN**********************************/
    cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());
    kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);


	std::vector<std::string> trainingImages = {
		//"tailA_1.png", "tailA_2.png", "tailA_3.png", "tailA_4.png", "tailA_5.png",
		//"tailB_1.png", "tailB_2.png", "tailB_3.png", "tailB_4.png",
		//"tailC_1.png", "tailC_2.png", "tailC_3.png", "tailC_4.png",
		"tailD_1.png",// "tailD_2.png", "tailD_3.png", "tailD_4.png",
		//"tailE_1.png", "tailE_2.png", 
		"tailE_3.png"//, "tailE_4.png",
		//"tailF_1.png", "tailF_2.png", "tailF_3.png", "tailF_4.png"
	};

	/*******************Processing Frame***************************/
	// takes a frame and process it for colors/shapes/alphanumerics
	for (auto& fName: trainingImages) // was for(;;)
	{
		/*******************Prep for Processing************************/
		std::vector<ContourWithData> allContoursWithData;
		std::vector<ContourWithData> validContoursWithData;
		std::vector<std::vector<cv::Point> > ptContours;        // declare contours vector
		std::vector<cv::Vec4i> v4iHierarchy;                    // declare contours hierarchy
		cv::Mat frameReference;

        //captRefrnc >> frameReference; TODO
        frameReference = cv::imread(fName);
        cv::Mat imgThresh = processFrame(frameReference, ptContours, v4iHierarchy);

        // Initialize allContoursWithData w/ boundingRect area
        for (int i = 0; i < ptContours.size(); i++) {
            ContourWithData contourWithData;
            contourWithData.ptContour = ptContours[i];
            contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
            contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);
            //contourWithData.moment = cv::moments(ptContours[i],false);
			// Preventing an invalid partition from being processed
			if (!contourWithData.checkIfContourIsValid()) continue;		// Moved to reduce contours total contours and partitions
            allContoursWithData.push_back(contourWithData);
        }

		// Preparing for contour partitions
        std::vector<int> labels;
		int r_tol = 3;
		float h_tol = 0.25;
        
		// Designating contour partitions via lambda filter
		int n_labels = cv::partition(allContoursWithData, labels, [r_tol,h_tol](const ContourWithData& lhs, const ContourWithData& rhs) {
			// Filter out by hight differentials
			if (!(std::abs(lhs.boundingRect.height - rhs.boundingRect.height) < lhs.boundingRect.height*h_tol)) return false;

			// Filter out by proximity
			if (!(lhs.boundingRect.x + lhs.boundingRect.width*2 > rhs.boundingRect.x 
				&& lhs.boundingRect.x - lhs.boundingRect.width*2 < rhs.boundingRect.x)) return false;
		});

		// Loading contour partitions
        std::vector< std::vector<ContourWithData> > contoursPartitioned(n_labels);
        for (int i = 0; i < allContoursWithData.size(); ++i) {
			// FROMERLY an ERROR on an undefined push_back, but fixed with an emplace_back
			contoursPartitioned[labels[i]].emplace_back(allContoursWithData[i]);
        }

		// Preparing to process contour partitions
		bool scannedTextTag = false;
		std::string strFinalString;

		// Process contour partitions
		for (int i = 0; i < contoursPartitioned.size(); i++) {
			if (contoursPartitioned[i].size() < 3) continue;

			// Check for Grouped Contours of x3 for Lettering
			if (scannedTextTag == false) {
				// sort contours from left to right
				std::sort(contoursPartitioned[i].begin(), contoursPartitioned[i].end(), ContourWithData::sortByBoundingRectXPosition);
				// Uses KNN and produces a tag inference
				processContours(kNearest, contoursPartitioned[i], frameReference, imgThresh, strFinalString);
				// Determines if strFinalString is appropirate
				if (!strFinalString.empty()) {
					scannedTextTag = true;
					break;
				}
			}

			// TODO: Check for Shapes Triangle / Trapezoid / Rectangle
			if (scannedTextTag) {
				//std::vector< std::vector<cv::Point> > approx;
				//approx.resize(validContoursWithData.size());
				//for (int k = 0; k < validContoursWithData.size(); k++) {
				//approxPolyDP(cv::Mat(validContoursWithData[k].ptContour), approx[k], 3, true);
				//}

			}
        }

        // show final string & input image with green boxes drawn around found digits
        std::cout << "tag read = " << strFinalString << "\n";
        cv::imshow(WIN_RF, frameReference);
        char c = (char)cv::waitKey(20);
        if (c == 27) break;
    }
    return 0;
}


void processContours(cv::Ptr<cv::ml::KNearest> &_kNearest, std::vector<ContourWithData> &_validContoursWithData, cv::Mat imgSrc, cv::Mat matThresh, std::string &_strFinalString)
{
	std::string validSubStrings = "UH8L6RG7CS1PJW3A2X";
    for (int i = 0; i < _validContoursWithData.size(); i++) {
		// Preventing an invalid partition from being processed
		//if (!_validContoursWithData[i].checkIfContourIsValid()) break; // TODO moved to init of allcontourswithdata

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

	// Last check for a valid tag
	if (_strFinalString.empty()) return;
	std::size_t found = validSubStrings.find(_strFinalString);
	if (found == std::string::npos || _strFinalString.size() != 3) _strFinalString.clear();
}

