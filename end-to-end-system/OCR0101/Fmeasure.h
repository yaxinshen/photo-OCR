#ifndef _FMEASURE_
#define _FMEASURE_
#include<string>
#include<vector>
#include "GroundAera.h"
#include <opencv2/opencv.hpp>
class Fmeasure{
public:
	std::string run(GroundAeraSet& cgts,GroundAeraSet& pgts);
	double PrecisionRecall(GroundAeraSet& cgts,GroundAeraSet& pgts);
	double getPrecisionRecall(GroundAeraSet& cgts,GroundAeraSet& pgts);
	double intersection(const cv::Rect& r1,const cv::Rect& r2);
};
#endif