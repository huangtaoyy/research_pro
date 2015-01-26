#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <string>
#include <cstring>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


string getData(int index);// read the data from the file,index specifies the index of the row
extern string startAddress; // the start address of the stack
static int blank = atoi(getData(2).c_str());// the blank of the stack
static int offset =atoi(getData(3).c_str());// the offset of the data should add
string getStr(long index,int data,int blank);
vector<int> getOperator(vector<int> ope,string exp);
//--------class instruction
class Instruction {
public:
	virtual void  fillInStack()=0;
	virtual string getStack(int);
	virtual ~Instruction();
	Instruction();
	virtual void fillInOperator(string str)=0;
	
public:
	
	int _ID;
	int _length;
	string _startAddress;
	vector<int> _operator;
	int result;
	vector<string> _instructionList;
	int _number;//instructionListµÄ¼ÆÊý
};

class ADD:public Instruction{
public:
	ADD();
//	~ADD();
	void fillInStack();
	void fillInOperator(string str);

};

class MINUS:public Instruction{
public:
	MINUS();
//	~MINUS();
	void fillInOperator(string str);
	void fillInStack();
};

class IF: public Instruction{
public:
//	IF();
//	~IF();
	void fillInStack();
	void fillInOperator(string str);

	Instruction* _insOne;
	Instruction* _insTwo;
	virtual string getStack(int);
private:
	int _ownNumber;
	
};

#endif

