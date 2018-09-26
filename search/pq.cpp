#include <iostream>
#include <queue>
#include <list>

using namespace std;

class Foo{
public:
	int x,y;
	Foo(){}
	Foo(int _x, int _y){
		this->x = _x;
		this->y = _y;
	}
};

class Compare{
public:
    bool operator()(Foo A, Foo B){    	
        return A.x>B.x;
    }
};

int main(){
    /*priority_queue<Foo, vector<Foo>, Compare> pq;
    pq.push(Foo(1,2));
    pq.push(Foo(2,2));
    pq.push(Foo(32,2));
    pq.push(Foo(4,2));
    pq.push(Foo(15,2));

    while(!pq.empty()){
    	cout << pq.top().x << endl;
    	pq.pop();
    }*/

    list<int> a;
    list<int>::iterator it;

    unsigned t = 20;
    for(unsigned i=0; i<t; i++) a.push_back(12);

    for(it=a.begin(); it!=a.end(); it++)
        cout << *it << "\t";
    cout << "\n";

    for(it=a.begin(); it!=a.end(); it++)
        if(*it > 10 and *it < 15){
            //cout << *it << "\t";
            it = a.erase(it);
        }


    for(it=a.begin(); it!=a.end(); it++)
        cout << *it << "\t";
    cout << "\n";

    return 0;
}