#ifndef _NODE_H_
#define _NODE_H_

static std::vector<std::pair<double, double> > vpoints;
static std::vector<std::pair<double, double> > vlines;

template<class G>
class cnode{
private:
	typedef typename G::node	node;
    typedef typename G::point	point;
    typedef typename G::H 		H;
    typedef typename G::W 		W;
    H m_hg;
public:
	std::list<node * > m_lady;	// ady's list
	point m_data;				// data node (point)
	bool m_state;
	unsigned m_id;
	
	cnode(point _data, unsigned _id){			// constructor with data
		this->m_data = _data;
		this->m_id = _id;
	}

	cnode(point _data){	this->m_data = _data;	}

	cnode(){}

	~cnode(){
		m_lady.clear();
	}

	void add_edge(node *e){		// add edge in ady's list
		this->m_lady.push_back(e);
		e->m_lady.push_back(this);
	}

	void save_data(){
		vpoints.push_back(std::make_pair(
			m_data.get_x(),m_data.get_y()));
	}

	void save_edge(){
		typename std::list<node *>::iterator it;
		for(it=m_lady.begin(); it!=m_lady.end(); it++){
			vlines.push_back(std::make_pair(m_data.get_x(),m_data.get_y()));
			vlines.push_back(std::make_pair((*it)->m_data.get_x(),(*it)->m_data.get_y()));
		}
	}

	void remove_lady(point a, point b){
		std::list<node *> tmp = m_lady;
		this->m_lady.clear();

		typename std::list<node *>::iterator it;
		for(it=tmp.begin(); it!=tmp.end(); it++){
			if(!(*it)->data_inside(a,b))
				this->m_lady.push_back(*it);
		}	
	}

	void print_lady(){		
		typename std::list<node *>::iterator it;
		for(it=m_lady.begin(); it!=m_lady.end(); it++){
			std::cout << (*it)->m_data << "\t";
		}
		std::cout << "\n";
	}

	bool data_inside(point a, point b){		
		return ((m_data.get_x()>a.get_x() and m_data.get_x()<b.get_x()) and
			(m_data.get_y()>a.get_y() and m_data.get_y()<b.get_y()));
	}

	W distance(node *t){
		return this->m_data.distance(t->m_data);
		 //(W)(sqrt(pow(m_data.get_x()-t->m_data.get_x(),2) + pow(m_data.get_y()-t->m_data.get_y(),2)));
	}

	void set_hg(H _hg){
		this->m_hg = _hg;
	}

	H get_hg(){	return this->m_hg;	}
};

#endif
