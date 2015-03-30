/*
 * main.cpp
 *
 *  Created on: 2013-12-21
 *      Author: huangtao
 */
#include <stdio.h>
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

void int_to_char(unsigned int addr, unsigned char* addr_arr, int size) {
	for (int i = 0; i < size; i++) {
		addr_arr[i] = addr >> (i * 8);
	}
}

void print_stack(stack<int> &s) {

	while (!s.empty()) {
		cout << s.top() << ", ";
		s.pop();
	}
	cout << endl;
}

void print_vector(vector<int> &v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		cout << (*it) << ",";
	}
	cout << endl;
}

int i = 0;

int func(int n) {
	static int a = 2;
	a ++;
	return a * n;
}

struct xx {
	long long x1;
	char x2;
	int x3;
	char x4[2];
	static int x5;
};
int xx::x5;

int main() {
//	int k = 5;
//	{
//		int i = 2;
//		k += func(i);
//	}
//
//	k += func(i);
//	cout << k << endl;

	int a = 3;
cout << sizeof(xx) << endl;
cout << sizeof(long long) << endl;

	return 0;

//	unsigned int a = 0x2bd5;
//	unsigned char b[4];
//	int_to_char(a, b, 4);
//
//	printf("a= %x, \t", a);
//	for (int i = 0; i < 4; i++) {
//		printf("b[%d] = %x, ", i, b[i]);
//	}
//
//	cout << endl;
//	stack<int> sa;
//	for (int i = 0; i < 4; i++) {
//		sa.push(i);
//	}
//
//	stack<int> sb;
//	sb = sa;
//
//	print_stack(sa);
////	print_stack(sb);
//
//	sb.push(6);
//
//	sa.push(4);
//
//	print_stack(sa);
//
//	print_stack(sb);

//	vector<int> v_a;
//	for (int i = 0; i < 10; i++) {
//		v_a.push_back(i + 1);
//	}
//	vector<int> v_b;
//	for (int i = 10; i < 20; i++) {
//		v_b.push_back(i + 1);
//	}
//	print_vector(v_b);
//	v_b.insert(v_b.end(), v_a.begin(), v_a.end());
//	print_vector(v_b);


}

