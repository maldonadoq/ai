#include <iostream>
#include "src/hopfield.h"

int main(int argc, char const *argv[]){	
	unsigned x = 10;
	unsigned y = 10;

	hopfield net("data/data.dat", x, y);
	net.calculate_weigth();

	ndata test("data/test.dat");
	test.load();

	unsigned i;
	for(i=0; i<test.correct.size(); i++)
		net.recognize(test.correct[i]);
	return 0;
}