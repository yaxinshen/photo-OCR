#ifndef _GROUNDAERA_
#define _GROUNDAERA_
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include "Tools.h"
class GroundAera{
public:
	GroundAera(){}
	GroundAera(std::string s,double xratio, double yratio);
	GroundAera(const cv::Rect& r);
private:
	cv::Rect rr;
	friend class GroundAeraSet;
	friend class Fmeasure;
};

class GroundAeraSet{
public:
	void display(cv::Mat& image);
	void push_back(GroundAera pgt){sets.push_back(pgt);}
	void clear(){sets.clear();}
	void segmentation(cv::Mat &image);
	int gasN;
private:
	std::vector<GroundAera>	sets;
	friend class Fmeasure;
	//void seg(cv::Mat &image,int i);
};


#endif
