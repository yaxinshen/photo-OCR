#include"GroundAera.h"
using namespace std;
using namespace cv;


GroundAera::GroundAera(std::string s,double xupdate,double yupdate)
{
	vector<cv::Point> vex;
	vector<std::string> ret = Tools::split(s," ");
    cv::Point pt = cv::Point(atoi(ret[0].c_str())*xupdate,atoi(ret[1].c_str())*yupdate);
	vex.push_back(pt);
    cv::Point pt1 = cv::Point(atoi(ret[2].c_str())*xupdate,atoi(ret[3].c_str())*yupdate);
	vex.push_back(pt1);
	rr =cv::boundingRect(vex);
}

GroundAera::GroundAera(const cv::Rect& r)
{
	this->rr = r;
}

void GroundAeraSet::display(cv::Mat& image)
{

	cv::Scalar s = cv::Scalar();s[0] = 0; s[1] = 0; s[2] = 255;
	for(int i = 0; i< this->sets.size();i++)
	{
		cv::Rect rec = sets[i].rr;
		cv::rectangle(image,rec, s,1,8,0);
	}
}

void GroundAeraSet::segmentation(cv::Mat& image)
{
	this->gasN = this->sets.size();
	for(int i = 0; i< this->sets.size();i++)
	{
		cv::Rect rec = sets[i].rr;
		if (rec.height/rec.width>3) 
		{rec.width = rec.height/1.5; 
		 if(rec.x - rec.height/4>0)
			 rec.x =rec.x - rec.height/4 ;}
		cv::Mat roi = image(rec);

                char file[50];
                if(i>=0&&i<=9)
		{sprintf(file,"./out/0%d.png",i) ;}
                else sprintf(file,"./out/%d.png",i) ;
		imwrite(file,roi);  
		//seg(roi,i);
	}
	
}


