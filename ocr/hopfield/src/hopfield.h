#ifndef _HOPFIELD_H_
#define _HOPFIELD_H_

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "ndata.h"

void print_character(std::vector<int> &in, std::vector<int> &out, unsigned x, unsigned y){
	unsigned i, j;
	for(i=0; i<x; i++){
		std::cout << " ";
		for(j=0; j<y; j++){
			if(in[(i*y)+j] == 1)	std::cout << '0';
			else	std::cout << ' ';
		}

		std::cout << "\t\t\t";
		for(j=0; j<y; j++){
			if(out[(i*y)+j] == 1)	std::cout << '0';
			else	std::cout << ' ';		
		}

		std::cout << "\n";
	}

	std::cout << "\n";
}

class hopfield{
private:		
	int *threshold;
	int **weight;
	std::vector<int> output;

	unsigned points;
	ndata input;	
public:
	unsigned x, y;
	unsigned nt;

	hopfield(std::string, unsigned, unsigned);	
	void calculate_weigth();
	int next_iteration(int);
	void recognize(std::vector<int> &);
	void asynchronous_correction();

	~hopfield(){	}
};

hopfield::hopfield(std::string _file, unsigned _x, unsigned _y){
	this->input = ndata(_file);
	if(!this->input.load()){
		std::cout << "Doesn't Exist this File!!\n";
	}
	
	this->x = _x;		// cols
	this->y = _y;		// rows

	this->points = _x*_y;
	this->output = std::vector<int>(points);
	this->threshold = new int[points];
	this->weight = new int*[points];

	for(unsigned i=0; i<points; i++){
		this->threshold[i] = 0;
		this->weight[i] = new int[points];
		for(unsigned j=0; j<points; j++)
			this->weight[i][j] = 0;
	}
}

void hopfield::calculate_weigth(){
	unsigned i,j,n;		// index loop
	int tw;				// temporal weight

	for(i=0; i<this->points; i++){
		for(j=0; j<this->points; j++){
			tw = 0;
			if(i != j){
				for(n=0; n<this->input.correct.size(); n++){
					// weight matrix					
					tw += input.correct[n][i]*input.correct[n][j];
				}					
			}

			this->weight[i][j] = tw;
		}
	}
}

int hopfield::next_iteration(int i){
	int out;
	unsigned j;
	int sum = 0;
	int ch =  0;

	for(j=0; j<this->points; j++){
		sum += this->weight[i][j]*this->output[j];
	}

	if(sum != this->threshold[i]){
		if(sum < this->threshold[i])	out = -1;
		if(sum > this->threshold[i])	out =  1;

		if(out != this->output[i]){
			ch = 1;
			this->output[i] = out;
		}
	}

	return ch;
}

void hopfield::asynchronous_correction(){
	int it  = 0;
	int lit = 0;

	do{
		it++;

		if(next_iteration(rand()%this->points)){
			lit = it;
		}
	} while((it-lit)<(int)(10*this->points));
}

// recognize the patter
void hopfield::recognize(std::vector<int> &nc){
	if(nc.size() != this->points){
		std::cout << "Size of input vector must be size of "
			<< this->points << "!!!\n";
		return;
	}

	unsigned i;

	for(i=0; i<this->points; i++)
		this->output[i] = nc[i];

	asynchronous_correction();

	print_character(nc, this->output, x, y);
}

#endif

// sata