#ifndef _NEURON_H_
#define _NEURON_H_

#include <utility>
#include <vector>
#include "activate.h"

float learning_rate = 0.01;

template<class W>
class cneuron{
private:
	std::vector<cneuron<W>> m_inputs;
	W *m_weights;	
	unsigned m_length;
public:
	W m_output, m_error;

	cneuron(W _error){	this->m_error = _error;	}
	cneuron(std::vector<cneuron<W>>_inputs){
		m_length = _inputs.size();
		m_inputs = new cneuron<W>[m_length];
		m_weights = new cneuron<W>[m_length];

		m_error = 0.0;

		for(unsigned i=0; i<m_length; i++){
			m_inputs[i] = _inputs[i];
			m_weights[i] =  (((double)rand()/(RAND_MAX))*2)-1;
		}
	}

	void response(){
		W input = 0;
		for(unsigned i=0; i<m_length; i++)
			input += m_inputs[i].m_output*m_weights[i];

		m_output = sigmoid(input);
		m_error = 0;
	}

	void set_error(W _desired){
		m_error = _desired-m_output;
	}

	void train(){
		W delta = (1-m_output)*(1+m_output)*m_error*learning_rate;
		for(unsigned i=0; i<m_length; i++){
			m_inputs[i].m_error += m_weights[i]*m_error;
			m_weights[i] += m_inputs[i].m_output*delta;
		}
	}

	std::pair<unsigned, W> get_strength(){
		unsigned idx, i;
		W str = 0.0;

		for(i=0; i<m_length; i++){
			if(m_weights[i] > str){
				idx = i;
				str = m_weights[i];
			}
		}

		return std::make_pair(idx, str);
	}

	void display(){
		std::cout << m_output << "\n";
	}
};

#endif