#include"Fmeasure.h"
using namespace std;
using namespace cv;


double Fmeasure::intersection(const Rect& r1,const Rect& r2)
{
	Rect rec = r1 & r2;
	return rec.area();
}


double Fmeasure::getPrecisionRecall(GroundAeraSet& cgts,GroundAeraSet& pgts)
{
	vector<GroundAera>& sets = cgts.sets;
	GroundAeraSet T = pgts;
	if(sets.size() == 0 || T.sets.size() == 0) return 0;
	double totalarea = 0;
	for(int i = 0; i< sets.size();i++)
		totalarea += sets[i].rr.area();
	double inter = PrecisionRecall(cgts,pgts);
	return inter/totalarea;
}

double Fmeasure::PrecisionRecall(GroundAeraSet& cgts,GroundAeraSet& pgts)
{
	vector<GroundAera>& sets = cgts.sets;
	GroundAeraSet T = pgts;
	double count = 0;
	for(int i = 0; i< sets.size();i++)
	{
		vector<double> store(T.sets.size());
		double maxval = 0;
		for(int j = 0; j< T.sets.size();j++)
		{
			double candval = intersection(sets[i].rr,T.sets[j].rr);
			if(candval > maxval) maxval = candval;
		}
		count = maxval + count;
	}
	return count;
}

std::string Fmeasure::run(GroundAeraSet& cgts,GroundAeraSet& pgts)
{
	double precison = getPrecisionRecall(cgts,pgts);
	double recall =	 getPrecisionRecall(pgts,cgts);
	double fmeasure;
	if(precison == 0 && recall == 0) fmeasure = 0;
	else fmeasure = 2*precison*recall/(recall + precison);
	char ret[100];
	sprintf(ret ,"Precision:%f,Recall:%f,Fmeasure:%f",precison,recall,fmeasure);
	return string(ret);
}