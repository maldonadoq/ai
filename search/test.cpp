#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <iterator>
#include <utility>
#include <math.h>

using namespace std;

template<class D>
class point{
public:
	D x,y;
	point(D _x, D _y){
		this->x = _x;
		this->y = _y;
	}
	point(){	}
};

template<class T, class V, class D>
class node{	
public:	
	T m_data;
	bool m_v;
	point<D> m_coord;
	list<pair<node<T,V,D> *, V> > lady;	
	D m_distance;
	node(T _m_data){	this->m_data = _m_data;	}	
	node(T _m_data, point<D> _m_coord){
		this->m_data = _m_data;
		this->m_coord = _m_coord;
	}

	friend ostream& operator<< (ostream & out, const node<T,V,D> &c){
		typename list<pair<node<T,V,D> *, V> >::iterator it;
		for(it=c.lady.begin(); it!=c.lady.end(); it++)
			out << (*it).first->m_data << "[" << (*it).second << "]\t";
		return out;
   	}
   	void set_ady(node<T,V,D> *a, V vl){
   		typename list<pair<node<T,V,D> *, V> >::iterator it;
   		for(it=lady.begin(); it!=lady.end(); it++)
			if(a == (*it).first)	return;

   		this->lady.push_back(make_pair(a,vl));
   	}
};

template<class T, class V, class D>
class compare{
public:
    bool operator()(node<T,V,D> *A, node<T,V,D> *B){
        return A->m_distance < B->m_distance;
    }
};

template<class T, class V, class D>
class graph{
private:
	vector<node<T,V,D> *> vnode;
public:
	graph(){};
	node<T,V,D> *find(T _d){
		for(unsigned i=0; i<this->vnode.size(); i++)
			if(this->vnode[i]->m_data == _d)	return this->vnode[i];
		return NULL;
	}
	
	void insert_node(T _d){	this->vnode.push_back(new node<T,V,D>(_d));	}
	void insert_node(T _d, D _x, D _y){	this->vnode.push_back(new node<T,V,D>(_d, point<D>(_x,_y)));	}
	
	void insert_edge(T _a, T _b, V _v, bool _t){
		node<T,V,D> *tma = find(_a);	
		node<T,V,D> *tmb = find(_b);

		if(tma == NULL or tmb == NULL)	return;

		if(_t)	tma->set_ady(tmb, _v);
		else{
			tma->set_ady(tmb, _v);
			tmb->set_ady(tma, _v);
		}
	}
	
	void print_edge(){
		for(unsigned i=0; i<this->vnode.size(); i++)
			cout << this->vnode[i]->m_data << ":\t" << *this->vnode[i] << endl;
		cout << endl;
	}

	void clean(node<T,V,D> *_t){
		for(unsigned i=0; i<this->vnode.size(); i++){
			this->vnode[i]->m_v = false;
			this->vnode[i]->m_distance = (D)(sqrt(pow(_t->m_coord.x-vnode[i]->m_coord.x,2)+
				pow(_t->m_coord.y-vnode[i]->m_coord.y,2)));
		}
	}

	void search(node<T,V,D> * _s, node<T,V,D> * _t, unsigned t){
		clean(_t);
		switch(t){
			case 1: depth_search(_s, _t);
				break;
			case 2: breadth_search(_s, _t);
				break;
			case 3: blind_point(_s, _t);
				break;
			case 4: hill_climbing(_s, _t);
				break;
		}
		cout << "\n";		
	}	

	void depth_search(node<T,V,D> *, node<T,V,D> *);
	void breadth_search(node<T,V,D> *, node<T,V,D> *);	
	void blind_point(node<T,V,D> *, node<T,V,D> *);
	void hill_climbing(node<T,V,D> *, node<T,V,D> *);
	void best_first(node<T,V,D> *, node<T,V,D> *);
	void a_asterisk(node<T,V,D> *, node<T,V,D> *);
	
	~graph(){};
};

// implement!!
template<class T, class V, class D>
void graph<T,V,D>::depth_search(node<T,V,D> * _s, node<T,V,D> * _t){
	stack<node<T,V,D> *> v;
	v.push(_s);

	node<T,V,D> *tmp;
	typename list<pair<node<T,V,D> *, V> >::iterator it;

	while(!v.empty()){
		tmp = v.top();				// use 		
		v.pop();
		cout << tmp->m_data << "\t";	// way
		for(it=tmp->lady.begin(); it!=tmp->lady.end(); it++){
			if(!(*it).first->m_v){
				(*it).first->m_v = true;		// visited
				v.push((*it).first);			// add node that was visited
			}
		}
	}
}

template<class T, class V, class D>
void graph<T,V,D>::breadth_search(node<T,V,D> * _s, node<T,V,D> * _t){
	queue<node<T,V,D> *> v;
	v.push(_s);

	node<T,V,D> *tmp;
	typename list<pair<node<T,V,D> *, V> >::iterator it;

	while(!v.empty()){
		tmp = v.front();				// use 
		cout << tmp->m_data << "\t";	// way
		for(it=tmp->lady.begin(); it!=tmp->lady.end(); it++){
			if(!(*it).first->m_v){
				(*it).first->m_v = true;		// visited
				v.push((*it).first);			// add node that was visited
			}
		}
		v.pop();
	}
}

template<class T, class V, class D>
void graph<T,V,D>::blind_point(node<T,V,D> * _s, node<T,V,D> * _t){
	list<node<T,V,D> *> v;
	v.push_front(_s);

	typename list<pair<node<T,V,D> *, V> >::iterator it;
	node<T,V,D> *tmp;
	srand(time(NULL));
	unsigned r;

	while(!v.empty()){
		tmp = v.front();
		v.pop_front();		
		cout << tmp->m_data << "\t";	// way		
		for(it=tmp->lady.begin(); it!=tmp->lady.end(); it++){
			if(!(*it).first->m_v){
				(*it).first->m_v = true;		// visited			
				if(v.size()==0)	v.push_back((*it).first);
				else{
					r = rand()%v.size();
					v.insert(next(v.begin(), r),(*it).first);
				}				
			}
		}
	}
}

template<class T, class V, class D>
void graph<T,V,D>::hill_climbing(node<T,V,D> *_s, node<T,V,D> *_t){
	priority_queue<node<T,V,D> *, vector<node<T,V,D> *>, compare<T,V,D>> pq;
	typename list<pair<node<T,V,D> *, V> >::iterator it;
	node<T,V,D> *tmp;

	//poner un stack y borrar pq por cada iteración!

	pq.push(_s);
	while(!pq.empty()){
		tmp = pq.top();
		pq.pop();
		cout << tmp->m_data << "\t";	// way
		for(it=tmp->lady.begin(); it!=tmp->lady.end(); it++){
			if(!(*it).first->m_v){
				(*it).first->m_v = true;		// visited			
				pq.push((*it).first);				
			}
		}
	}
}

typedef graph<char, int, float> grph;	// graph!!
typedef node<char, int, float> nod;	// node!!

int main(int argc, char const *argv[]){
	grph *a = new grph();
	bool d = true;

	a->insert_node('A',1,5);	a->insert_node('B',5,10);
	a->insert_node('C',10,15);	a->insert_node('D',15,9);
	a->insert_node('E',5,5);	a->insert_node('F',10,10);
	a->insert_node('G',5,0);	a->insert_node('H',10,4);

	a->insert_edge('A','B',1,d);
	a->insert_edge('A','E',2,d);
	a->insert_edge('A','G',3,d);
	a->insert_edge('B','C',5,d);
	a->insert_edge('B','F',2,d);
	a->insert_edge('B','H',5,d);
	a->insert_edge('G','H',7,d);
	a->insert_edge('C','D',3,d);
	a->insert_edge('F','D',6,d);
	a->insert_edge('H','D',9,d);

	nod *source = a->find('A');
	nod *target = a->find('H');

	a->search(source, target, 1);
	a->search(source, target, 2);
	a->search(source, target, 3);
	a->search(source, target, 4);

	delete source;	delete target;	delete a;	
	return 0;
}

/*

	void create_edge(){
		unsigned i,j,nr,nc;
		nr = this->m_nrow-1;
		nc = this->m_ncol-1;
		
		for(j=0; j<nc; j++){
			this->m_vnodes[0][j]->add_edge(this->m_vnodes[0][j+1]);
			this->m_vnodes[nr][j]->add_edge(this->m_vnodes[nr][j+1]);
		}

		for(i=0; i<nr; i++){
			this->m_vnodes[i][0]->add_edge(this->m_vnodes[i+1][0]);
			this->m_vnodes[i][nc]->add_edge(this->m_vnodes[i+1][nc]);
		}

		this->m_vnodes[0][1]->add_edge(this->m_vnodes[1][0]);
		this->m_vnodes[0][nc-1]->add_edge(this->m_vnodes[1][nc]);
		this->m_vnodes[nr][1]->add_edge(this->m_vnodes[nr-1][0]);
		this->m_vnodes[nr][nc-1]->add_edge(this->m_vnodes[nr-1][nc]);

		for(i=1; i<nr; i++){
			for(j=1; j<nc; j++){
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i-1][j-1]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i-1][j]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i-1][j+1]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i][j-1]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i][j+1]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i+1][j-1]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i+1][j]);
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i+1][j+1]);
			}			
		}
	}
*/