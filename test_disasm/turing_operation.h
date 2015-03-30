/*
 * turing_operation.h
 *
 *  Created on: 2015-3-24
 *      Author: huangtao
 */

#ifndef TURING_OPERATION_H_
#define TURING_OPERATION_H_

#include <vector>
#include <string>
#include "trie_tree.h"

//Load & Store
class Load_Store {
public:
	//reg-eax by default
	void load_constant_to_register(vector<LGadget*>& l_gadgets, LGadget* &l_g);
	void load_constant_to_register(vector<LGadget*>& l_gadgets, string &reg, LGadget* &l_g);

	void load_from_memory(vector<LGadget*>& l_gadgets);
	void store_to_memory(vector<LGadget*>& l_gadgets);
};


#endif /* TURING_OPERATION_H_ */
