#ifndef _DTREE_H_
#define _DTREE_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <map>

#include "table.h"
#include "node.h"

class dtree{
private:
	dtable m_initial;
	std::vector<dnode> m_tree;
public:

	dtree(){	}
	dtree(dtable _table){
		this->m_initial = _table;
		this->m_initial.extract_attr_value();

		dnode root;
		root.m_tree_idx = 0;
		this->m_tree.push_back(root);

		run(m_initial, 0);
	}

	void run(dtable, int);
	std::pair<std::string, int> get_majority_label(dtable);
		
	int    get_select_attr(dtable);
	bool   is_leaf_node(dtable);
	double get_gain_ratio(dtable, int);
	double get_info_d(dtable);
	double get_info_attr_d(dtable, int);
	double get_gain(dtable, int);
	double get_split_info_attr_d(dtable, int);

	std::string guess(std::vector<std::string>);
	int dfs(std::vector<std::string >&, int);

	~dtree(){
		this->m_tree.clear();
	}
};

void dtree::run(dtable _table, int _nodeidx){
	if(is_leaf_node(_table)){
		m_tree[_nodeidx].m_is_leaf = true;
		m_tree[_nodeidx].m_label = _table.m_data.back().back();
		return;
	}

	int select_attr_idx = get_select_attr(_table);

	std::map<std::string, std::vector<int> > attr_value_map;
	unsigned i, j;
	for(i=0; i<_table.m_data.size(); i++){
		attr_value_map[_table.m_data[i][select_attr_idx]].push_back(i);
	}

	m_tree[_nodeidx].m_crit_attr_idx = select_attr_idx;
	std::pair<std::string, int> majority = get_majority_label(_table);
	if((double)majority.second/_table.m_data.size() > 0.8) {
		m_tree[_nodeidx].m_is_leaf = true;
		m_tree[_nodeidx].m_label = majority.first;
		return;
	}

	std::string attr_value;	

	for(i=0; i<m_initial.m_attr_value[select_attr_idx].size(); i++){
		dtable tnext;
		dnode nnext;


		attr_value = m_initial.m_attr_value[select_attr_idx][i];

		std::vector<int> cand = attr_value_map[attr_value];
		for(j=0; j<cand.size(); j++){
			tnext.m_data.push_back(_table.m_data[cand[j]]);
		}		

		nnext.m_attr_value = attr_value;
		nnext.m_tree_idx = (int)m_tree.size();

		m_tree[_nodeidx].m_children.push_back(nnext.m_tree_idx);
		m_tree.push_back(nnext);

		if(tnext.m_data.size() == 0){
			nnext.m_is_leaf = true;
			nnext.m_label = get_majority_label(_table).first;
			m_tree[nnext.m_tree_idx] = nnext;
		}
		else{
			run(tnext, nnext.m_tree_idx);
		}
	}
}

std::pair<std::string, int> dtree::get_majority_label(dtable _table) {
	std::string mlabel = "";
	int mcount = 0;

	std::map<std::string, int> lcount;

	for(unsigned i=0;i< _table.m_data.size(); i++) {
		lcount[_table.m_data[i].back()]++;

		if(lcount[_table.m_data[i].back()] > mcount) {
			mcount = lcount[_table.m_data[i].back()];
			mlabel = _table.m_data[i].back();
		}
	}

	return {mlabel, mcount};
}

bool dtree::is_leaf_node(dtable _table){
	for(unsigned i=1; i < _table.m_data.size(); i++){
		if(_table.m_data[0].back() != _table.m_data[i].back())
			return false;
	}

	return true;
}

int dtree::get_select_attr(dtable _table) {
	int max_attr_idx = -1;
	double max_attr_value = 0.0;

	for(unsigned i=0; i< m_initial.m_name.size()-1; i++) {
		if(max_attr_value < get_gain_ratio(_table, i)) {
			max_attr_value = get_gain_ratio(_table, i);
			max_attr_idx = i;
		}
	}

	return max_attr_idx;
}

double dtree::get_gain_ratio(dtable _table, int attr_idx) {
	return get_gain(_table, attr_idx)/get_split_info_attr_d(_table, attr_idx);	
}

double dtree::get_info_d(dtable _table) {
	double ret = 0;
	int icount = (int)_table.m_data.size();
	std::map<std::string, int> lcount;

	for(unsigned i=0; i<_table.m_data.size();i++) {
		lcount[_table.m_data[i].back()]++;
	}

	for(auto it=lcount.begin(); it != lcount.end(); it++) {
		double p = (double)it->second/icount;

		ret += -1.0 *p*log(p)/log(2);
	}

	return ret;
}

double dtree::get_info_attr_d(dtable _table, int attr_idx) {
	double ret = 0.0;
	int icount = (int)_table.m_data.size();

	std::map<std::string, std::vector<int> > attr_value_map;
	for(unsigned i=0; i<_table.m_data.size(); i++) {
		attr_value_map[_table.m_data[i][attr_idx]].push_back(i);
	}

	for(auto it=attr_value_map.begin(); it != attr_value_map.end(); it++) {
		dtable tnext;
		for(unsigned i=0; i<it->second.size(); i++) {
			tnext.m_data.push_back(_table.m_data[it->second[i]]);
		}
		
		int next_icount = (int)tnext.m_data.size();

		ret += (double)next_icount/icount * get_info_d(tnext);
	}

	return ret;
}

double dtree::get_gain(dtable _table, int attr_idx){
	return get_info_d(_table)-get_info_attr_d(_table, attr_idx);
}

double dtree::get_split_info_attr_d(dtable _table, int attr_idx) {
	double ret = 0.0;

	int icount = (int)_table.m_data.size();

	std::map<std::string, std::vector<int> > attr_value_map;

	unsigned i;

	for(i=0;i<_table.m_data.size();i++) {
		attr_value_map[_table.m_data[i][attr_idx]].push_back(i);
	}

	for(auto it=attr_value_map.begin(); it != attr_value_map.end(); it++) {
		dtable tnext;
		for(i=0; i<it->second.size(); i++) {
			tnext.m_data.push_back(_table.m_data[it->second[i]]);
		}
		int nicount = (int)tnext.m_data.size();

		double d = (double)nicount/icount;
		ret += -1.0 * d * log(d) / log(2);
	}

	return ret;
}

std::string dtree::guess(std::vector<std::string> row){

	std::string label = "";
	int leaf_node = dfs(row, 0);

	if(leaf_node == -1){
		return "failed!";
	}

	label = m_tree[leaf_node].m_label;
	return label;
}

int dtree::dfs(std::vector<std::string >& _row, int _here){
	if(m_tree[_here].m_is_leaf)
		return _here;

	int crit_attr_idx = m_tree[_here].m_crit_attr_idx;

	int next;
	for(unsigned i=0; i<m_tree[_here].m_children.size(); i++){
		next = m_tree[_here].m_children[i];

		if(_row[crit_attr_idx] == m_tree[next].m_attr_value){
			return dfs(_row, next);
		}
	}

	return -1;
}

#endif