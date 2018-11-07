#ifndef _NDATA_H_
#define _NDATA_H_

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

		getline(tfile, stmp);		
		boost::split(vtmp,stmp,boost::is_any_of("|"));
		unsigned i,j;
		std::vector<int> ntmp;

		for(i=0; i<vtmp.size(); i++){
			stmp = vtmp[i];
			for(j=0; j<stmp.size(); j++)
				ntmp.push_back((stmp[j]=='0')? -1: 1);
			correct.push_back(ntmp);
			ntmp.clear();
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