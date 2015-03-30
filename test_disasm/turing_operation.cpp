/*
 * turing_operation.cpp
 *
 *  Created on: 2015-3-24
 *      Author: huangtao
 */
#include "turing_operation.h"

//reg-eax by default
void Load_Store::load_constant_to_register(vector<LGadget*>& l_gadgets,
		LGadget* &l_g) {

	string reg = "eax";
	load_constant_to_register(l_gadgets, reg, l_g);
}

void Load_Store::load_constant_to_register(vector<LGadget*>& l_gadgets,
		string &reg, LGadget* &l_g) {

	vector<LGadget*>::iterator it = l_gadgets.begin();
	for (; it != l_gadgets.end(); ++it) {
		if ((*it)->is_gadget_valid()) {

			vector<Inst*>::iterator vi = (*it)->insts.begin();
			for (; vi != (*it)->insts.end(); ++vi) {
				if ((*vi)->inst_valid && (*vi)->node->insn->type == insn_pop) {

					vector<x86_reg_t>::iterator wi = (*vi)->node->write_set.begin();
					for (; wi != (*vi)->node->write_set.end(); ++ wi) {
						if (strcasecmp(reg.c_str(), (*wi).name) == 0) {
							//find a l_gadget
							l_g = (*it);
							return;
						}
					}

				}
			}

		}
	}
}

void Load_Store::load_from_memory(vector<LGadget*>& l_gadgets) {

}

void Load_Store::store_to_memory(vector<LGadget*>& l_gadgets) {

}