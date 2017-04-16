#ifndef _GROUND_DETECT_
#define _GROUND_DETECT_
#include <string>
#include "Candidate.h"
#include "GroundAera.h"
#include "GetCandidate.h"
#include "ValidCandidate.h"
#include "Fmeasure.h"
#include <opencv2/opencv.hpp>

///Main class for text detection
class GroundDetect{
public:
	void readImageGroundTruth(const cv::Mat& image,const std::vector<std::string> gt);
	void run();
	void showGroundAeraSet();
	std::string calculateResult();
	void showsegmentation();
	int candiNum ;
private:
	GetCandidate getCandidate;///class for get all characters from input image
	ValidCandidate validCandidate;///class for link characters into text lines
	Fmeasure fmeasure;//class for calculating the F-measure based on cgts and pgts

	cv::Mat oriBgrImage_8UC3;///Copy of the input Image
	std::vector<Candidate> candidateStore;///Character set calculated by the system
	GroundAeraSet cgts;///The Rect region calculated by the system
	GroundAeraSet pgts;///The Rect region transform by gt
	std::string PRF;///precision,recall,F-measure
};

#endif
