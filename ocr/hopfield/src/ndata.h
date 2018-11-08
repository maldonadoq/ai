#ifndef _NDATA_H_
#define _NDATA_H_

#include <vector>

void string_to_vector(std::vector<int> &ntmp, const std::string stmp){
	unsigned j;
	for(j=0; j<stmp.size(); j++)
		ntmp.push_back((stmp[j]=='.')? -1: 1);
}

class ndata{
public:
	std::vector<std::vector<int> > correct;
	std::string file;
	ndata(std::string _file){	this->file = _file;	}
	ndata(){	}

	bool load(){
		std::vector<std::string> vtmp;
		std::string stmp;

		std::ifstream tfile(this->file);
		if(!tfile.is_open())
			return false;

		//getline(tfile, stmp);

		std::string line;
	    while(getline(tfile, line)){
	        stmp += line;
	    }
	    
		boost::split(vtmp,stmp,boost::is_any_of("|"));
		unsigned i;
		std::vector<int> ntmp;

		for(i=0; i<vtmp.size(); i++){
			stmp = vtmp[i];
			ntmp.clear();
			string_to_vector(ntmp, stmp);
			correct.push_back(ntmp);			
		}

		return true;
	}

	void print(){
		unsigned i,j;
		for(i=0; i<correct.size(); i++){
			for(j=0; j<correct[i].size(); j++)
				std::cout << correct[i][j];
			std::cout << "\n";
		}
	}
};

#endif