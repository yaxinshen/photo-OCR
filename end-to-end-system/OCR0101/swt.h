#ifndef _SWT_H_
#define _SWT_H_

#include "Candidate.h"
#include <iostream>
#include <vector>
#include <set>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct Vpoint{
   int i;
   int j;
   float val;
};

class Squeue
{
public:
	int length; 
	int inport;
	int outport;
	Vpoint* str;
	Vpoint* originalstr;
	 
	 Squeue(){
		str = new Vpoint[3072000];
		for(int i = 0;i<307200;i++)
		{
			str[i].i = 0;
			str[i].j = 0;
			str[i].val = 0;
		}
		inport = 0;
		outport = 0;
		length = inport - outport;
	 };

	 ~Squeue(){delete str;};

	 void push(Vpoint& m){
		str[inport].i = m.i;
		str[inport].j = m.j;
		str[inport].val = m.val;
		inport += 1;
		length = inport - outport;
	 };

	 void pop (Vpoint& m){
		m.i = str[outport].i;
		m.j = str[outport].j;
		m.val = str[outport].val;
		outport += 1;
		length = inport - outport;
	 };

	 void push_v(Vpoint& m){
		str[inport].i = m.i;
		str[inport].j = m.j;
		inport += 1;
		length = inport - outport;
	 };

	 void pop_v(Vpoint& m){
		m.i = str[outport].i;
		m.j = str[outport].j;
		outport += 1;
		length = inport - outport;
	 };

	 void reset(){
		inport = 0;
		outport = 0;
		length = inport - outport;
	 };

};

class Swt{
public:
	Swt();
	void initialize(Mat& image,vector< vector<cv::Point2i> >& cc);
	void getStrokeWidth(vector<cv::Point2i>& component, Candidate& cad);
	void gradient();
	void inversegradient();
	void Edges(vector< vector<cv::Point2i> >& cc);
	void td_findPairLine();
	void displayStrokeWidth(vector< vector<cv::Point2i> >& cc);

private:
	cv::Mat StrokeImage;
	cv::Mat GrayImage;
	cv::Mat EdgeImage;
	cv::Mat max_x;
	cv::Mat max_y;
	int height;
	int width;
	Squeue* qbtc;
	Squeue* qb; ///for rgb
};

#endif
