#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <iostream>

template<class T>
void print_vector(std::vector<T> vect){
    for(unsigned i=0; i<vect.size(); i++){
        std::cout << vect[i] << "\t";
    }
    std::cout << "\n";

    //std::cout << vect.size() << "\n";
}

#endif