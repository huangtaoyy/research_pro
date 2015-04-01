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
	bool load_constant_to_register(vector<LGadget*>& l_gadgets, LGadget* &l_g);
	bool load_constant_to_register(vector<LGadget*>& l_gadgets, string &reg, LGadget* &l_g);

	bool load_from_memory(vector<LGadget*>& l_gadgets, string& src_reg, string& des_reg, LGadget* &l_g);
	bool store_to_memory(vector<LGadget*>& l_gadgets, string& src_reg, string& des_reg, LGadget* &l_g);

	void val_dbug(x86_op_t* op);
	int count_valid_inst(LGadget* &l_g);
};


#endif /* TURING_OPERATION_H_ */
