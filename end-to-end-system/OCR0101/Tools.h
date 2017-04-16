#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <string>
#include <vector>
//for fmeasure and others.
class Tools{
public:
	static std::vector<std::string> split(std::string str,std::string pattern){
		std::string::size_type pos;
		std::vector<std::string> result;
		str+=pattern;
		int size=str.size();

		for(int i=0; i<size; i++)
		{
			pos=str.find(pattern,i);
			if(pos<size)
			{
				std::string s=str.substr(i,pos-i);
				result.push_back(s);
				i=pos+pattern.size()-1;
			}
		}
		return result;
	};
	static double min(double a1, double a2){double ret = (a1 < a2)?a1:a2; return ret;}
	static double max(double a1, double a2){double ret = (a1 > a2)?a1:a2; return ret;}
};


#endif