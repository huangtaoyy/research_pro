/*
 * inst_dependency.cpp
 *
 *  Created on: 2015-3-19
 *      Author: huangtao
 */
#include "inst_dependency.h"
#include <set>
#include <vector>

void ferase() {
	set<int> myset;
	vector<int> myvec;

	myset.erase(myset.begin());
	myvec.erase(myvec.begin());
}
//#include <algorithm>
//
////instruction is accept is not stack pointer, base pointer, segment and immediate
//void Inst::accept_inst() {
//	x86_oplist_t* op_list = node->insn->operands;
//	is_accept = i_accept;
//
//	while (op_list != NULL) {
//		x86_op_t* op = &op_list->op;
//
//		switch (op->type) {
//		case op_immediate:
//			is_accept = i_refuse;
//			break;
//		case op_register:
//			if (strcmp(op->data.reg.name, "ebp") == 0
//					|| strcmp(op->data.reg.name, "esp") == 0) {
//				is_accept = i_refuse;
//			}
//			break;
//		case op_relative_near:
//		case op_relative_far:
//		case op_absolute:
//		case op_offset:
//		case op_expression:
//		case op_unused:
//		case op_unknown:
//			break;
//		default:
//			break;
//		}
//	}
//	op_list = op_list->next;
//}
//
//
//void set_reg_name(vector<x86_reg_t> &v, vector<string> &v_s) {
//	for (vector<x86_reg_t>::iterator it = v.begin(); it != v.end(); ++it) {
//		v_s.push_back((*it).name);
//	}
//}
//
////if two vector has a same object
//bool has_same_reg(vector<x86_reg_t> &v1, vector<x86_reg_t> &v) {
//	vector<string> v1_s, v_s;
//	set_reg_name(v1, v1_s);
//	set_reg_name(v, v_s);
//
//	for (vector<string>::iterator i1 = v1_s.begin(); i1 != v1_s.end(); ++i1) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), *i1);
//		if (r != v_s.end()) {
//			return true;
//		}
//	}
//	return false;
//}
//
////registers altered before ins_i
//void LGadget::pre_write(vector<x86_reg_t> & result, size_t i) {
//	size_t count = 1;
//
//	vector<Inst*>::iterator it = insts.begin();
//	for (; it != insts.end() && count < i && i <= insts.size() && i > 1; ++it) {
//		TrieNode *node = (*it)->node;
//		for (vector<x86_reg_t>::iterator v_i = node->write_set.begin();
//				v_i != node->write_set.end(); ++v_i) {
//			result.push_back(*v_i);
//		}
//		count++;
//	}
//}
//
////registers altered after ins_i
//void LGadget::post_write(vector<x86_reg_t> & result, size_t i) {
//	vector<Inst*>::iterator it = insts.begin();
//	for (it += i + 1; it != insts.end(); ++it) {
//		TrieNode *node = (*it)->node;
//		for (vector<x86_reg_t>::iterator v_i = node->write_set.begin();
//				v_i != node->write_set.end(); ++v_i) {
//			result.push_back(*v_i);
//		}
//	}
//}
//
////no conflict
//bool LGadget::is_inst_valid(size_t i) {
//	Inst* inst = insts.at(i - 1);
//
//	if (inst->is_accept == i_refuse) {
//		return false;
//	}
//
//	//readset
//	vector<x86_reg_t> read_set = inst->node->read_set;
//
//	//writeset
//	vector<x86_reg_t> write_set = inst->node->write_set;
//
//	//prewrite
//	vector<x86_reg_t> pre_write_v;
//	pre_write(pre_write_v, i);
//
//	if (has_same_reg(read_set, pre_write_v)) {
//		return false;
//	}
//	if (has_same_reg(write_set, pre_write_v)) {
//		return false;
//	}
//
//	//postwrite
//	vector<x86_reg_t> post_write_v;
//	post_write(post_write_v, i);
//
//	if (has_same_reg(write_set, post_write_v)) {
//		return false;
//	}
//
//	return true;
//}
//
////A LGadget is G-Valid if there exists at least one I-Valid instruction in it
//bool LGadget::is_gadget_valid() {
//	for (size_t i; i != insts.size(); ++i) {
//		if (is_inst_valid(i)) {
//			return true;
//		}
//	}
//	return false;
//}

//bool has_same_reg(vector<string>& v_s, string str) {
//	//eax
//	if (strcasecmp(str.c_str(), "eax") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "al");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ah");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ax");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "ax") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "al");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ah");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "eax");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "ah") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "al");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ax");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "eax");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "al") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ah");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ax");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "eax");
//		if (r != v_s.end()) {
//			return true;
//		}
//	}
//
//	//ebx
//	if (strcasecmp(str.c_str(), "ebx") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bh");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "bx") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bh");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ebx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "bh") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bx");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ebx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "bl") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bh");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "bx");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ebx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	}
//
//	//ecx
//	if (strcasecmp(str.c_str(), "ecx") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "cl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ch");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "cx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "cx") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "cl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ch");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ecx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "ch") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "cl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "cx");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ecx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "cl") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ch");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "cx");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "ecx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	}
//
//	//edx
//	if (strcasecmp(str.c_str(), "edx") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dh");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "dx") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dh");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "edx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "dh") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dl");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dx");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "edx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	} else if (strcasecmp(str.c_str(), "dl") == 0) {
//		vector<string>::const_iterator r = find(v_s.begin(), v_s.end(), str);
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dh");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "dx");
//		if (r != v_s.end()) {
//			return true;
//		}
//		r = find(v_s.begin(), v_s.end(), "edx");
//		if (r != v_s.end()) {
//			return true;
//		}
//	}
//
//	return false;
//}
