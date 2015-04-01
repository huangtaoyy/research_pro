/*
 * trie_tree.cpp
 *
 *  Created on: 2015-3-16
 *      Author: huangtao
 */

#include <algorithm>
#include "trie_tree.h"

string TrieNode::RET_STR = "ret";

Trie::Trie() {
	root = new TrieNode;
	root->insn_str = TrieNode::RET_STR;
	root->gadget.push(new TrieNode("ret"));
	count_c3 = 0;
	count_gadget = 0;
}

string Trie::make_plural(int count, const string &word, const string &ending) {
	return (count == 1) ? word : word + ending;
}

void Trie::output(string & filename) {
	fout.open(filename.c_str(), ios::out);
	if (!fout.is_open()) {
		cout << "output.gad open failed!" << endl;
		return;
	}

	output(root);

//	fout.seekg(0, ios::beg);
//	fout << "there are " << count_c3 << make_plural(count_c3, " ret", "s") << endl;
//
//	fout.seekg(0, ios::beg);
//	fout << "there are " << count_gadget << make_plural(count_gadget, " gadget", "s") << endl;

	cout << "there are " << count_c3 << make_plural(count_c3, " ret", "'s")
			<< endl;
	cout << "there are " << count_gadget
			<< make_plural(count_gadget, " gadget", "s") << endl;
	fout.close();
}

void Trie::output(TrieNode* parent) {
	if (parent->record.empty()) {
		print_stack(parent->gadget);
		return;
	}

	vector<TrieNode*>::iterator it = parent->record.begin();
	for (; it != parent->record.end(); it++) {
		output((TrieNode*) *it);
	}
}

void Trie::print_stack(stack<TrieNode*> &s) {
	if (s.size() > 8 || s.size() < 3) {
		return;
	}

	//another 3<=length<=7 gadget, increment gadget number
	count_gadget++;
	LGadget* l_g = new LGadget;

	while (!s.empty()) {
		TrieNode* x = s.top();
		fout << x->insn_str << endl;
//		cout << x->insn_str << endl;
//		new Inst(x);
		l_g->insts.push_back(new Inst(x));
		s.pop();
	}

	//put LGadget into a vector
	l_gadgets.push_back(l_g);

//	cout << endl;
	fout << endl;

}

dep_inst_type Inst::is_inst_accept(x86_op_t* op) {
	if (op != NULL) {
		switch (op->type) {
		case op_immediate:
			return i_refuse;
		case op_register:
			if (strcasecmp(op->data.reg.name, "ebp") == 0
					|| strcmp(op->data.reg.name, "esp") == 0) {
				return i_refuse;
			}
//			cout << op->data.reg.name << " : " << op->data.reg.type << endl;
			break;
		case op_expression:
			if (strcasecmp(op->data.expression.base.name, "ebp") == 0
					|| strcasecmp(op->data.expression.base.name, "esp") == 0
					|| strcasecmp(op->data.expression.index.name, "ebp") == 0
					|| strcasecmp(op->data.expression.index.name, "esp") == 0) {
				return i_refuse;
			}
//			cout << op->data.expression.base.name << " : "
//					<< op->data.expression.base.type << endl;
//			cout << op->data.expression.index.name << " : "
//					<< op->data.expression.index.type << endl;
			break;
		case op_unused:
		case op_unknown:
			return i_refuse;
		case op_relative_near:
		case op_relative_far:
		case op_absolute:
		case op_offset:

		default:
			break;
		}
	}

	return i_accept;
}

//instruction is accept is not stack pointer, base pointer, segment and immediate
dep_inst_type Inst::is_inst_accept() {

	if (is_inst_accept(x86_operand_1st(node->insn)) == i_refuse) {
		return i_refuse;
	}

	if (is_inst_accept(x86_operand_2nd(node->insn)) == i_refuse) {
		return i_refuse;
	}

	if (is_inst_accept(x86_operand_3rd(node->insn)) == i_refuse) {
		return i_refuse;
	}

	return i_accept;
}

void set_reg_name(vector<x86_reg_t> &v, vector<string> &v_s) {
	for (vector<x86_reg_t>::iterator it = v.begin(); it != v.end(); ++it) {
		v_s.push_back((*it).name);
	}
}

bool has_same_reg1(vector<string>& v_s, string str) {
	static const int col = 4;
	static const int row = 12;
	static string regs[row][col] = { { "eax", "ax", "ah", "al" }, { "ebx", "bx",
			"bh", "bl" }, { "ecx", "cx", "ch", "cl" },
			{ "edx", "dx", "dh", "dl" }, { "edi", "di", "di", "di" }, { "esi",
					"si", "si", "si" }, { "[eax]", "[ax]", "[ah]", "[al]" }, { "[ebx]",
					"[bx]", "[bh]", "[bl]" }, { "[ecx]", "[cx]", "[ch]", "[cl]" }, { "[edx]",
					"[dx]", "[dh]", "[dl]" }, { "[edi]", "[di]", "[di]", "[di]" }, { "[esi]",
					"[si]", "[si]", "[si]" } };

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (strcasecmp(regs[i][j].c_str(), str.c_str()) == 0) {
				for (int k = 0; k < col; k++) {
					vector<string>::const_iterator r = find(v_s.begin(),
							v_s.end(), regs[i][k].c_str());
					if (r != v_s.end()) {
						return true;
					}
				}
				return false;
			}
		}
	}

	return false;
}

//erase those more than once
void erase_reduplicated(vector<string>& v_s) {
	sort(v_s.begin(), v_s.end());
	vector<string>::iterator it = unique(v_s.begin(), v_s.end());
	v_s.erase(it, v_s.end());
}

//if two vector has a same object
bool has_same_reg(vector<string> &v1, vector<string> &v) {
	if (v.empty() || v1.empty()) {
		return false;
	}

//	vector<string> v1_s, v_s;
//	set_reg_name(v1, v1_s);
	erase_reduplicated(v1);
//	set_reg_name(v, v_s);
	erase_reduplicated(v);

	for (vector<string>::iterator i1 = v1.begin(); i1 != v1.end(); ++i1) {
		if (has_same_reg1(v, *i1)) {
			return true;
		}
	}
	return false;
}

//registers altered before ins_i
void LGadget::pre_write(vector<string> & result, size_t i) {
	if (i > insts.size() || i < 1) {
		return;
	}
	size_t count = 0;

	vector<Inst*>::iterator it = insts.begin();
	for (; it != insts.end() && count < i; ++it) {
		TrieNode *node = (*it)->node;

		result.insert(result.end(), node->write_set.begin(),
				node->write_set.end());
		count++;
	}
}

//registers altered after ins_i
void LGadget::post_write(vector<string> & result, size_t i) {
	if (i >= insts.size()) {
		return;
	}
	vector<Inst*>::iterator it = insts.begin() + i + 1;
	for (; it != insts.end(); ++it) {
		TrieNode *node = (*it)->node;

		result.insert(result.end(), node->write_set.begin(),
				node->write_set.end());
	}
}

//no conflict
bool LGadget::is_inst_valid(size_t i) {
	Inst* inst = insts.at(i);

	if (inst->is_inst_accept() == i_refuse) {
		return false;
	}

	//readset
	vector<string> read_set = inst->node->read_set;

	//writeset
	vector<string> write_set = inst->node->write_set;

	//prewrite
	vector<string> pre_write_v;
	pre_write(pre_write_v, i);

	if (has_same_reg(read_set, pre_write_v)) {
		return false;
	}
	if (has_same_reg(write_set, pre_write_v)) {
		return false;
	}

	//postwrite
	vector<string> post_write_v;
	post_write(post_write_v, i);

	if (has_same_reg(write_set, post_write_v)) {
		return false;
	}

	inst->inst_valid = true;

	return true;
}

//A LGadget is G-Valid if there exists at least one I-Valid instruction in it
bool LGadget::is_gadget_valid() {
	int count_valid = 0;
	for (size_t i = 0; i != insts.size(); ++i) {
		if (strcasecmp(insts[i]->node->insn_str.c_str(),
				TrieNode::RET_STR.c_str()) != 0 && is_inst_valid(i)) {
			count_valid++;
		}
	}
	if (count_valid > 0) {
		return true;
	}

//	cout << "exit is_gadget_ not valid!" << endl;
	return false;
}

void LGadget::print_insts() {
	cout << endl;
	vector<Inst*>::iterator it = insts.begin();
	for (; it != insts.end(); ++it) {
		cout << (*it)->node->insn_str
				<< ((*it)->inst_valid ? ":\tvalid" : ":\tnon") << endl;
	}
	cout << endl;
}

void LGadget::write_insts(fstream& fout) {
	fout << endl;
	vector<Inst*>::iterator it = insts.begin();
	for (; it != insts.end(); ++it) {
		fout << (*it)->node->insn_str
				<< ((*it)->inst_valid ? ":\tvalid" : ":\tnon") << endl;
	}
	fout << endl;
}
