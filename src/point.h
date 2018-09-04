#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>

template<class G>
class cpoint{
private:
	typedef typename G::T 		T;
	typedef typename G::point	point;
	T x, y;
public:
	cpoint(T _x, T _y){
		this->x = _x;
		this->y = _y;
	}

	cpoint(){}

	friend std::ostream& operator<< (std::ostream & out, point &c){
		out << "(" << c.get_x() << "," << c.get_y() << ")";	
		return out;
   	}

   	friend bool operator==(point &a, point &b){
       	return (a.get_x() == b.get_x()) and (a.get_y() == b.get_y());
   	}

	T get_x(){	return this->x;	}
	T get_y(){	return this->y;	}

	void set_x(T _x){	this->x = _x;	}
	void set_y(T _y){	this->y = _y;	}
};

#endif