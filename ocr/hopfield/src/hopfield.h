#ifndef _HOPFIELD_H_
#define _HOPFIELD_H_

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <vector>
#include "ndata.h"

class hopfield{
private:
	int points;
	int *output;
	int *threshold;
	int **weight;
	ndata input;	
public:
	unsigned x, y;

	hopfield(std::string _file, unsigned _x, unsigned _y){
		this->input = ndata(_file);
		if(!this->input.load()){
			std::cout << "Doesn't Exist this File!!\n";
		}
		
		this->x = _x;
		this->y = _y;			
	}

	~hopfield(){	}
};

#endif