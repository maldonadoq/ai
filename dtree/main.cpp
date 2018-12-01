#include <iostream>
#include <string>

#include "src/utils.h"
#include "src/dtree.h"

// 1train.dat from http://archive.ics.uci.edu/ml/datasets/Car+Evaluation
// 2train.dat from slide 15

int main(int argc, char const *argv[]){
	std::string strain = "data/1train.dat";
	std::string stest = "data/1test.dat";	

	dtable train;
	train.load_from_csv(strain);

	// std::cout << train << "\n";

	dtable test;
	test.load_from_csv(stest);

	// std::cout << test << "\n";

	dtree tree(train);

	dtable result;
	result.set_name(train.get_name());

	std::vector<std::string > tmp;

	for(unsigned i=0; i<test.get_data_size(); i++){
		tmp = test.get_irow_data(i);
		tmp.push_back(tree.guess(test.get_irow_data(i) ) );
		result.add_row_data(tmp);
	}

	std::cout << result;

	return 0;
}