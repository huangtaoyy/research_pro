#include "algorithm.h"
#include <iostream>

void test() {

	string s = "(a,b),ADD(3,4),MINUS(6,5)";

	int indexOne = s.find("),");

	int indexTwo = s.find("),", indexOne + 2);
	cout << indexOne << ", " << indexTwo << endl;
}
int main() {

	test();
	Instruction *add = new ADD;
	add->fillInOperator("34,5");
	add->fillInStack();
	for (int i = 0; i < add->_number; i++) {
		cout << add->getStack(i) << endl;
		;
	}

	Instruction * minus = new MINUS;

	minus->fillInOperator("50,40");
	minus->fillInStack();
	//Instruction *minus = new MINUS;
	/*
	 map.add("+",add);
	 map.add("_",add);
	 while(){

	 Instruction ins = map.get(temp);
	 for (int i = 0;i<add->_number;i++)
	 {
	 snprintf(..,ins->getStack(i));
	 }
	 }

	 */
	cout << getStr(0, 1017709, 3) << endl;
	system("pause");
}
