#include <iostream>
#include "src/hopfield.h"

int main(int argc, char const *argv[]){	
	unsigned x = 10;
	unsigned y = 10;
	hopfield tmp("data/data.dat", x, y);
	return 0;
}