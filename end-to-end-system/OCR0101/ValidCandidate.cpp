#include"ValidCandidate.h"
#include<iostream>
#include<fstream>

using namespace std;

GroundAeraSet ValidCandidate::run(std::vector<Candidate>& candidateStore)
{
	this->candidateStore = candidateStore;
	cgts.clear();
	link();
	finalresult();
	return cgts;
}
void ValidCandidate::finalresult()
{
	for(int i = 0;i< CandidateNum;i++)
	{
		bool sign = false;
		for(int j = 0;j< CandidateNum;j++)
			sign = sign | relation[i][j];
		candidateStore[i].alive = sign;
		candidateStore[i].chuan = 0;
	}

	int index = 1;
	int tempNum;
	for(int i = 0;i< CandidateNum;i++)
	{
		if(candidateStore[i].alive == false)
		{continue;}
		Iq->reset();
		Iq->push(i);
		candidateStore[i].chuan = index;
		candidateStore[i].alive = false;
		while(Iq->length)
		{
			tempNum = Iq->pop();
			for(int j = 0;j< CandidateNum;j++)
			{
				if(relation[tempNum][j] == true)
				{
					Iq->push(j);
					candidateStore[j].chuan = index;
					candidateStore[j].alive = false;
					relation[tempNum][j] = false;
					relation[j][tempNum] = false;
				}
			}

		}
		index++;
	}

	vector< vector<int> > store(index + 2);
	for(int i = 0;i< CandidateNum;i++)
	{
		store[candidateStore[i].chuan].push_back(i);
	}
	//cout << "candidate: " << CandidateNum<<endl;
	//cout << "index: " << index<<endl;
	ofstream outtofile;
	outtofile.open("./paixu.txt");
	//int linepx = 0;
	for(int i = 1;i< index;i++)
	{
		//cout << "store[" << i << "]"<<"size:" << store[i].size()<<endl;
		if(store[i].size() <=3)
			continue;

		vector<cv::Point> PointV;
		int storecf[256] = {0};
		for(int t = 0; t < store[i].size(); t++)
			for(int j = t; j< store[i].size();j++)
			{
				if(t != j){
				double minv = abs(candidateStore[store[i][t]].middle_i - candidateStore[store[i][j]].middle_i);
				double maxv = abs(candidateStore[store[i][t]].middle_j - candidateStore[store[i][j]].middle_j);
				double Theroid = Tools::min(candidateStore[store[i][t]].high,candidateStore[store[i][j]].high);

				double Theroid2 = Tools::min(candidateStore[store[i][t]].width,candidateStore[store[i][j]].width);
				if(maxv < 0.05*Theroid2 && minv < 0.05*Theroid) {

					storecf[j] = t+1;
				}

				}

			}

		int output[80];	int rr = 0;
		int op[80];
		for(int j = 0; j< store[i].size();j++)
		{
			//cout << "storecf"<<storecf[j] <<endl;

			if(!storecf[j]){
			//if ( j == 0) linepx = candidateStore[store[i][j]].middle_i;
			vector<cv::Point> PointV;
			int a = (candidateStore[store[i][j]].max_j - candidateStore[store[i][j]].min_j)	/ 10;
			int b = (candidateStore[store[i][j]].max_i - candidateStore[store[i][j]].min_i)	/ 10;
            PointV.push_back(cv::Point(candidateStore[store[i][j]].min_j-a,candidateStore[store[i][j]].min_i-b));
			PointV.push_back(cv::Point(candidateStore[store[i][j]].max_j+a,candidateStore[store[i][j]].max_i+b));
			cv::Rect textline = cv::boundingRect(PointV);
			cgts.push_back(GroundAera(textline));
			output[rr] = candidateStore[store[i][j]].middle_j;
			op[rr]=rr;
			rr++;
			}
		}
		
		int min = 0;
		for(int ii = 0; ii < rr; ii++){
			for(int jj = ii; jj < rr; jj++){
				 if( output[jj] < output[ii] ) {
					int tmp = output[ii];
					output[ii] = output[jj];
					output[jj] = tmp;
					tmp = op[ii];
					op[ii] = op[jj];
					op[jj] = tmp;
				 }
			}
		}
		 // linepx = 
		//outtofile << linepx << endl;
			for(int ii = 0; ii < rr; ii++)
			{//cout<<op[ii]<<" ";	  
		      outtofile << op[ii] << " ";}
			outtofile << '\n';		
	}
}

void ValidCandidate::findPairRegion(int i, int j)
{
	relation[i][j] = false;
	relation[j][i] = false;
	if(candidateStore[i].middle_j == candidateStore[j].middle_j && candidateStore[i].middle_i == candidateStore[j].middle_i) return;
	if(candidateStore[i].high/candidateStore[j].high > 1.5 || candidateStore[i].high/candidateStore[j].high < 0.75) return;
	if(candidateStore[i].width/candidateStore[j].width > 4 || candidateStore[i].width/candidateStore[j].width < 0.25) return;
	if(candidateStore[i].strokeWidth/candidateStore[j].strokeWidth > 3 || candidateStore[i].strokeWidth/candidateStore[j].strokeWidth < 0.333) return;
	if(abs(candidateStore[i].avergecolor - candidateStore[j].avergecolor) > 20 ) return;
	if(abs(candidateStore[i].averBlue - candidateStore[j].averBlue) > 20 ) return;
	if(abs(candidateStore[i].averRed - candidateStore[j].averRed) > 20 ) return;
	if(abs(candidateStore[i].averGreen - candidateStore[j].averGreen) > 20 ) return;
	double minv = abs(candidateStore[i].middle_i - candidateStore[j].middle_i);
	double maxv = abs(candidateStore[i].middle_j - candidateStore[j].middle_j);
	double Theroid = Tools::min(candidateStore[i].high,candidateStore[j].high);
	if(maxv > 4*Theroid || minv > 0.4*Theroid) return ; 

	/*double Theroid2 = Tools::min(candidateStore[i].width,candidateStore[j].width);
	if(maxv > 4*Theroid2 || maxv < 0.4*Theroid2) return ; 
	if(minv > 4*Theroid || minv < 0.4*Theroid) return ; */

	this->relation[i][j] = true;
	this->relation[j][i] = true;
}

void ValidCandidate::link() 
{
	CandidateNum = candidateStore.size();
	for(int i = 0;i< CandidateNum - 1;i++)
		for(int j = i + 1;j< CandidateNum;j++)
			findPairRegion(i,j);
}

