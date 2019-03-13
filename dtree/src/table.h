#ifndef _TABLE_H_
#define _TABLE_H_

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "utils.h"

class dtable{
public:

	std::vector<std::string > m_name;						// col name
	std::vector<std::vector<std::string> > m_data;			// col data
	std::vector<std::vector<std::string> > m_attr_value;	// col value

	//dtable(){}	

	void extract_attr_value();
	bool load_from_csv(std::string);
	bool add_row_data(std::vector<std::string> );

	std::vector<std::string> get_name();
	void set_name(std::vector<std::string>);

	unsigned get_data_size();
	std::vector<std::string> get_irow_data(unsigned);

	~dtable(){
		this->m_name.clear();
		this->m_data.clear();
		this->m_attr_value.clear();
	}

	friend std::ostream& operator<< (std::ostream & out, const dtable &c){
		unsigned i, j;
		for(i=0; i<c.m_name.size(); i++){
			out << c.m_name[i] << "\t";
		}
		out << "\n";

		for(i=0; i<c.m_data.size(); i++){
			for(j=0; j<c.m_data[i].size(); j++){
				out << c.m_data[i][j] << "\t";
			}
			out << "\n";
		}

       return out;
   	}
};

void dtable::extract_attr_value(){ 
	m_attr_value.resize(m_name.size());

	for(unsigned i=0; i<m_name.size(); i++){
		std::map<std::string, int> value;

		for(unsigned j=0; j<m_data.size(); j++){
			value[m_data[j][i]] = 1;
		}

		for(auto it = value.begin(); it != value.end(); it++){
			m_attr_value[i].push_back(it->first);
		}
	}
}

bool dtable::load_from_csv(std::string filename){	    
	std::ifstream file(filename);
	if(!file.is_open())
		return false;

	std::vector<std::string> row;
	std::string line;

	getline(file, line);
	boost::split(row,line,boost::is_any_of("\t"));

	this->m_name = row;
	while(getline(file, line)){
		boost::split(row,line,boost::is_any_of("\t"));
		
		this->m_data.push_back(row);
	}

	return true;
}

std::vector<std::string> dtable::get_name(){
	return this->m_name;
}

void  dtable::set_name(std::vector<std::string> rows){
	this->m_name = rows;
}

unsigned dtable::get_data_size(){
	return this->m_data.size();
}

std::vector<std::string> dtable::get_irow_data(unsigned i){
	return this->m_data[i];
}

bool dtable::add_row_data(std::vector<std::string> row){
	if(row.size() != this->m_name.size()){
		return false;
	}

	this->m_data.push_back(row);
	return true;
}

#endif