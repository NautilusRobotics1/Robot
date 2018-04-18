// GenData.cpp

#include "openCV_Lib.h"
int main() {
    cv::Mat imgTrainingNumbers;

    std::vector<std::vector<cv::Point> > ptContours;
    std::vector<cv::Vec4i> v4iHierarchy;

    cv::Mat matClassificationInts;
    cv::Mat matTrainingImagesAsFlattenedFloats;

    // possible chars we are interested in are digits 0 through 9 and capital letters A through Z, put these in vector intValidChars
    std::vector<int> intValidChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z' };

    // Needs ValidShapes {Triangle, Trapezoid, Rectangle};
    // Needs ValidColor {Red,Blue,Yellow};

	std::vector<std::string> trainingImages = {
		//"tailA_1.png", "tailA_2.png", "tailA_3.png", "tailA_4.png",
		//"tailB_1.png", "tailB_2.png", "tailB_3.png", "tailB_4.png",
		//"tailC_1.png"//, "tailC_2.png", "tailC_3.png", "tailC_4.png",
		//"tailD_1.png", "tailD_2.png", "tailD_3.png", "tailD_4.png",
		//"tailE_1.png", "tailE_2.png", "tailE_3.png", "tailE_4.png",
		"tailF_1.png"//, "tailF_2.png", "tailF_3.png", "tailF_4.png",
		//"training_chars.png"
	};

	// Training KNN
	for (auto &fName : trainingImages) // access by reference to avoid copying
	{
		imgTrainingNumbers = cv::imread(fName);
		cv::imshow("imgTrainingNumbers", imgTrainingNumbers);
		cv::Mat imgThresh = processFrame(imgTrainingNumbers, ptContours, v4iHierarchy);

		// for each contour
		for (int i = 0; i < ptContours.size(); i++) {
			// if contour is big enough to consider
			if (cv::contourArea(ptContours[i]) < MIN_CONTOUR_AREA)
				continue;

			// get the bounding rect
			cv::Rect boundingRect = cv::boundingRect(ptContours[i]);
			// draw red rectangle around each contour as we ask user for input
			cv::rectangle(imgTrainingNumbers, boundingRect, cv::Scalar(0, 0, 255), 2);
			// get ROI image of bounding rect
			cv::Mat matROI = imgThresh(boundingRect);

			// resize image, this will be more consistent for recognition and storage
			cv::Mat matROIResized;
			cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

			cv::imshow("matROI", matROI);
			cv::imshow("matROIResized", matROIResized);
			// show training numbers image, this will now have red rectangles drawn on it
			cv::imshow("imgTrainingNumbers", imgTrainingNumbers);

			// get key press
			int intChar = cv::waitKey(0);

			// if esc key == 27 was pressed, exit program or if . key == 47 was pressed skip contour
			if (intChar == 27) return(0);
			else if (intChar == 46) continue;
			else if (std::find(intValidChars.begin(), intValidChars.end(), intChar) != intValidChars.end()) {
				// else if the char is in the list of chars we are looking for . . .
				
				// append classification char to integer list of chars
				matClassificationInts.push_back(intChar);

				// now add the training image (some conversion is necessary first) . . .
				cv::Mat matImageFloat;
				// convert Mat to float
				matROIResized.convertTo(matImageFloat, CV_32FC1);

				// flatten
				cv::Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);

				// add to Mat as though it was a vector, this is necessary due to the data types that KNearest.train accepts
				matTrainingImagesAsFlattenedFloats.push_back(matImageFlattenedFloat);
			}
		}
	}

	// Finish Up
    std::cout << "training complete\n\n";
    //uploadTrainingData(matClassificationInts, matTrainingImagesAsFlattenedFloats);
    return 0;
}

