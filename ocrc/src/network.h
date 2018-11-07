#ifndef _NETMORK_H_
#define _NETMORK_H_

#include <vector>
#include "neuron.h"
#include "card.h"

template<class D, class W>
class cnetwork{
private:
	std::vector<cneuron<W> *> m_input_layer;
	std::vector<cneuron<W> *> m_hidden_layer;
	std::vector<cneuron<W> *> m_output_layer;
	unsigned m_best;
public:
	cnetwork(unsigned inputs, unsigned hidden, unsigned outputs){
		this->m_input_layer = std::vector<cneuron<W>>(inputs);
		this->m_hidden_layer = std::vector<cneuron<W>>(hidden);
		this->m_output_layer = std::vector<cneuron<W>>(outputs);

		unsigned i;
		for(i=0; i<inputs; i++)		m_input_layer[i] = new cneuron<W>();
		for(i=0; i<hidden; i++)		m_hidden_layer[i] = new cneuron<W>();
		for(i=0; i<outputs; i++)	m_output_layer[i] = new cneuron<W>();
	}

	void response(ccard card){
		unsigned i;
		for(i=0; i<m_input_layer.size(); i++)
			m_input_layer[i]->m_output = card.m_inputs[i];

		for(i=0; i<m_hidden_layer.size(); i++)
			m_hidden_layer[i]->respond();

		for(i=0; i<m_output.size(); i++)
			m_output_layer[i]->respond();
	}

	void display(){
		for(unsigned i=0; i<m_output_layer.size(); i++){
			m_output_layer[i]->display();
		}
		std::cout << "\n";
	}

	void train(std::vector<cneuron<W> *> outputs){
		unsigned i;
		for(i=0; i<m_output_layer.size(); i++){
			m_output_layer[i]->set_error(outputs[i]);
			m_output_layer[i]->train();
		}

		W tmp = -1.0;
		for(i=0; i<m_output_layer.size(); i++){
			if(m_output_layer[i]->m_output > tmp)
				m_best = i;
		}

		// back propagation
		for(i=0; i<m_hidden_layer.size(); i++)
			m_hidden_layer[i]->train();
	}
};

#endif
