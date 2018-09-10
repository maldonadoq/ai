#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <stack>
#include <queue>

#include "point.h"
#include "node.h"

extern std::vector<std::pair<double, double> > vpoints;
extern std::vector<std::pair<double, double> > vlines;
static std::vector<std::pair<double, double> > vpatha;
static std::vector<std::pair<double, double> > vpathd;
static std::vector<std::pair<double, double> > vpathb;

template<class T>
class compare{
public:
    bool operator()(T A, T B){
        return A->get_hg() > B->get_hg();
    }
};

// regular matrix
template<class G>
class cgraph{
private:
	typedef typename G::node	node;	// class node
    typedef typename G::point	point;	// class point
    typedef typename G::W 		W;
    typedef typename G::H 		H;

	unsigned m_srow, m_scol;	// size in pixel of row and col
	unsigned m_nrow, m_ncol;	// number of row and col of regular matrix
	unsigned m_unit;			// minimun unit
	node ***m_vnodes;			// matrix of cnodes
public:
	cgraph(unsigned _srow, unsigned _scol, unsigned _unit){
		this->m_srow = _srow;
		this->m_scol = _scol;
		this->m_unit = _unit;
		this->m_nrow = (this->m_srow/this->m_unit)+1;
		this->m_ncol = (this->m_scol/this->m_unit)+1;

		this->m_vnodes = new node**[this->m_nrow];
		for(unsigned i=0; i<this->m_nrow; i++)
			this->m_vnodes[i] = new node*[this->m_ncol];
	}

	cgraph(){}

	void create_regular_vertex(){
		unsigned i,j,k=0;
		for(i=0; i<this->m_nrow; i++){
			for(j=0; j<this->m_ncol; j++){
				this->m_vnodes[i][j] = new node(point(this->m_unit*j, this->m_unit*i), k++);
			}
		}
	}

	void create_regular_edge(){
		unsigned i,j,nr,nc;
		nr = this->m_nrow-1;
		nc = this->m_ncol-1;			

		for(i=0; i<nr; i++){
			for(j=0; j<nc+1; j++){
				this->m_vnodes[i][j]->add_edge(this->m_vnodes[i+1][j]);
				if(j<nc){
					this->m_vnodes[i][j]->add_edge(this->m_vnodes[i+1][j+1]);
					this->m_vnodes[i+1][j]->add_edge(this->m_vnodes[i][j+1]);
					this->m_vnodes[i][j]->add_edge(this->m_vnodes[i][j+1]);
				}
			}
		}

		for(j=0; j<nc; j++)
			this->m_vnodes[nr][j]->add_edge(this->m_vnodes[nr][j+1]);
	}

	void create_regular_graph(){
		create_regular_vertex();
		create_regular_edge();
	}

	void print_vertex_data(){
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){
				if(this->m_vnodes[i][j] != NULL)
					std::cout << "(" << this->m_vnodes[i][j]->m_data.get_x() << "," 
						<< this->m_vnodes[i][j]->m_data.get_y() << ")\t";
			}
			std::cout << "\n";
		}
	}	

	void save_to_opengl(){
		vpoints.clear();
		vlines.clear();
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){				
				if(this->m_vnodes[i][j] != NULL){
					this->m_vnodes[i][j]->save_data();
					this->m_vnodes[i][j]->save_edge();
				}
			}
		}
	}

	void remove_node_in(point a, point b){		
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){
				if(this->m_vnodes[i][j] != NULL){
					this->m_vnodes[i][j]->remove_lady(a,b);				
				}
			}
		}

		node *tmp;
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){
				if(this->m_vnodes[i][j] != NULL and this->m_vnodes[i][j]->data_inside(a,b)){
					tmp = this->m_vnodes[i][j];
					this->m_vnodes[i][j] = NULL;
					delete tmp;			
				}
			}
		}
		save_to_opengl();
	}

	node* find_node(point d){
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){
				if(this->m_vnodes[i][j] != NULL and 
					this->m_vnodes[i][j]->m_data == d)
					return this->m_vnodes[i][j];
			}
		}
		return NULL;
	}

	void restart_target(){
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){
				if(this->m_vnodes[i][j] != NULL){
					this->m_vnodes[i][j]->m_state = false;
				}
			}
		}
	}

	void restart_path(std::vector<std::pair<double, double> > &pt, point **cf, point src, point tar){				
		pt.clear();
		
		bool t = true;
		unsigned tx,ty;
		point current = tar;

		pt.push_back(std::make_pair(current.get_x(),current.get_y())); 

		while(current != src){
			tx = current.get_x()/m_unit;
			ty = current.get_y()/m_unit;

			current = cf[ty][tx];
			pt.push_back(std::make_pair(current.get_x(),current.get_y())); 
			//std::cout << current << "\t";
		}
	}

	void a_asterisk(node *s, node *t){
		restart_target();		

		typename std::list<node *>::iterator it;
		std::priority_queue<node *, std::vector<node *>, compare<node *> > pq;		

		pq.push(s);
		node* current;

		H h;
		W g;

		unsigned tx,ty, i;
		point **camefrom = new point*[m_nrow];		
		for(i=0; i<m_nrow; i++)	camefrom[i] = new point[m_ncol];

		while(!pq.empty()){
			current = pq.top();
			pq.pop();
		
			if(current == t){
				restart_path(vpatha, camefrom, s->m_data, t->m_data);
				return;
			}

			for(it=current->m_lady.begin(); it!=current->m_lady.end(); it++){
				if(!(*it)->m_state){
					(*it)->m_state = true;
					h = (*it)->distance(t);
					g = current->distance(*it);
					(*it)->set_hg(h+g);
					pq.push(*it);


					tx = (*it)->m_data.get_x()/m_unit;
					ty = (*it)->m_data.get_y()/m_unit;

					camefrom[ty][tx] = point(current->m_data.get_x(), current->m_data.get_y());
				}
			}
		}

		delete []camefrom;
	}

	void depth_search(node* s, node* t){
		restart_target();

		std::stack<node *> v;
		v.push(s);
		
		node *current;
		typename std::list<node* >::iterator it;

		unsigned tx,ty, i;
		point **camefrom = new point*[m_nrow];		
		for(i=0; i<m_nrow; i++)	camefrom[i] = new point[m_ncol];

		while(!v.empty()){
			current = v.top();				// use 		
			v.pop();
			
			if(current == t){
				restart_path(vpathd, camefrom, s->m_data, t->m_data);
				return;
			}

			for(it=current->m_lady.begin(); it!=current->m_lady.end(); it++){
				if(!(*it)->m_state){
					(*it)->m_state = true;		// visited
					v.push(*it);

					tx = (*it)->m_data.get_x()/m_unit;
					ty = (*it)->m_data.get_y()/m_unit;

					camefrom[ty][tx] = point(current->m_data.get_x(), current->m_data.get_y());
				}
			}
		}
	}


	void breadth_search(node* s, node* t){
		restart_target();

		std::queue<node *> v;
		v.push(s);

		node *current;
		typename std::list<node* >::iterator it;

		unsigned tx,ty, i;
		point **camefrom = new point*[m_nrow];		
		for(i=0; i<m_nrow; i++)	camefrom[i] = new point[m_ncol];

		while(!v.empty()){
			current = v.front();
			
			if(current == t){
				restart_path(vpathb, camefrom, s->m_data, t->m_data);
				return;
			}

			for(it=current->m_lady.begin(); it!=current->m_lady.end(); it++){
				if(!(*it)->m_state){
					(*it)->m_state = true;		// visited
					v.push(*it);

					tx = (*it)->m_data.get_x()/m_unit;
					ty = (*it)->m_data.get_y()/m_unit;

					camefrom[ty][tx] = point(current->m_data.get_x(), current->m_data.get_y());
				}
			}
			v.pop();
		}
	}

	node* near_to(point a){
		node* tmp = NULL;
		W min_dist = 100000;
		W tdist;
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){				
				if(this->m_vnodes[i][j] != NULL){
					tdist = this->m_vnodes[i][j]->m_data.distance(a);
					if(tdist < min_dist){						
						tmp = this->m_vnodes[i][j];
						min_dist = tdist;
					}
				}
			}
		}
		
		return tmp;
	}

	~cgraph(){
		for(unsigned i=0; i<this->m_nrow; i++){
			for(unsigned j=0; j<this->m_ncol; j++){
				if(this->m_vnodes[i][j] != NULL){
					this->m_vnodes[i][j] = NULL;
				}
			}
		}

		delete []this->m_vnodes;
	}
};

template<class _T, class _W, class _H>
class traits{
public:
    typedef traits<_T, _W, _H>	self;
    typedef _T			T;
    typedef _W			W;
    typedef _H			H;

    typedef cpoint<self>	point;
    typedef cgraph<self>	graph;
    typedef cnode<self>		node;
};

#endif