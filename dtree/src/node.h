#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <vector>

class dnode{
public:

	int m_crit_attr_idx;
	std::string m_attr_value;

	int m_tree_idx;
	bool m_is_leaf;
	std::string m_label;

	std::vector<int> m_children;

	dnode(){
		this->m_is_leaf = false;
	}

	~dnode(){
		this->m_children.clear();
	}
};

#endif