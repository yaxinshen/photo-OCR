#ifndef _VALIDCANDIDATE_
#define _VALIDCANDIDATE_
#include"Candidate.h"
#include "GroundAera.h"
#include "Tools.h"
#include <vector>
class Vqueue
{
public:
	int length;
	int inport;
	int outport;
	int *str;

	Vqueue(){
	length = 0;
	inport = 0;
	outport = 0;
	str = new int[3000];
};
	~Vqueue();

	int pop(){
	outport += 1;
	length = inport - outport;
	return str[outport - 1];
	};
	
	void push(int n){
	str[inport] = n;
    inport += 1;
	length = inport - outport;
	};
	
	void reset(){
	length = 0;
	inport = 0;
	outport = 0;
	};

};

class ValidCandidate{
public:
	ValidCandidate(){this->Iq = new Vqueue;relation = std::vector< std::vector<bool> >(3000,std::vector<bool>(3000,true)); }
	GroundAeraSet run(std::vector<Candidate>& allCandidate);
	void findPairRegion(int i, int j);
	void finalresult();
	void link();
private:
	std::vector<Candidate> candidateStore;
	int CandidateNum;
	std::vector< std::vector<bool> > relation;
	Vqueue* Iq;
	GroundAeraSet cgts;
};

#endif
