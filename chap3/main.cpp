/*
 * main.cpp
 *
 *  Created on: 2015-1-21
 *      Author: huangtao
 */

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>

using namespace std;

double Hermit_Iterative(int n, double x) {
	if (n == 0) {
		return 1;
	} else if (n == 1) {
		return 2 * x;
	} else {
		double res1 = 1, res2 = 2 * x;
		double result = 0;
		for (int i = 2; i <= n; i++) {
			result = res2 * 2 * x - res1 * 2 * (i - 1);
			res1 = res2;
			res2 = result;
		}
		return result;
	}
}

double Hermit_Recursive(int n, double x) {
	if (n == 0) {
		return 1;
	} else if (n == 1) {
		return 2 * x;
	} else {
		return 2 * x * Hermit_Recursive(n - 1, x)
				- 2 * (n - 1) * Hermit_Recursive(n - 2, x);
	}
}

void test_hermit() {
	const int n = 3;
	double x = 3.14;
	cout << "Hermit(): " << endl;
	cout << Hermit_Iterative(n, x) << endl << Hermit_Recursive(n, x) << endl;
}

int Ack(int m, int n) {
	if (m == 0) {
		return n + 1;
	} else if (n == 0) {
		return Ack(m - 1, 1);
	} else {
		return Ack(m - 1, Ack(m, n - 1));
	}
}

void test_ack() {
	int m = 3, n = 3;
	cout << "Ack(): " << endl;
	cout << Ack(m, n) << endl;
}

void print_arr(int *a, int n) {
	for (int i = 0; i < n; i++) {
		cout << a[i] << ", ";
	}
	cout << endl;
}

void swap_mn(int* a, int m, int n) {

	for (int i = 0; i < m; i++) {
		int temp = a[0];
		for (int j = 1; j < m + n; j++) {
			a[j - 1] = a[j];
		}
		a[m + n - 1] = temp;
	}
}

void test_swap() {
	int a[] = { 1, 2, 3, 4 };
	print_arr(a, 4);
	swap_mn(a, 3, 1);
	print_arr(a, 4);
}

int strlen2(const char s[]) {
	int n = 0;
	for (const char *p = s; *p != '\0'; p++)
		n++;
	return n;
}

char* strcpy2(char dst[], const char src[]) {
	char *p1;
	const char* p2;
	for (p1 = dst, p2 = src; *p2 != '\0'; p1++, p2++) {
		*p1 = *p2;
	}
	*p1 = '\0';
	return dst;
}

char *strncpy2(char dst[], const char src[], int n) {
	char *p1;
	const char *p2;
	for (p1 = dst, p2 = src; n != 0 && *p2 != '\0'; p1++, p2++, n--) {
		*p1 = *p2;
	}
	if (n != 0) {
		*p1 = '\0';
	}
	return dst;
}

char *strcat2(char dst[], const char src[]) {
	char *p1;
	const char *p2;
	for (p1 = dst; *p1 != '\0'; p1++)
		;
	for (p2 = src; *p2 != '\0'; p1++, p2++)
		*p1 = *p2;
	*p1 = '\0';
	return dst;
}

char *strncat2(char dst[], const char src[], int n) {
	char *p1;
	const char *p2;
	for (p1 = dst; *p1 != '\0'; p1++)
		;
	for (p2 = src; n != 0 && *p2 != '\0'; p1++, p2++, n--)
		*p1 = *p2;
	if (n != 0)
		*p1 = '\0';
	return dst;
}

int strcmp2(const char s1[], const char s2[]) {
	const char *p1 = s1, *p2 = s2;
	for (; *p1 != '\0' && *p2 != '\0'; p1++, p2++) {
		if (*p1 > *p2)
			return 1;
		else if (*p1 < *p2)
			return -1;
	}
	if (*p1 == '\0' && *p2 == '\0')
		return 0;
	else if (*p1 == '\0')
		return -1;
	else
		return 1;
}

int strncmp2(const char s1[], const char s2[], int n) {
	const char *p1 = s1, *p2 = s2;
	for (; n != 0 && *p1 != '\0' && *p2 != '\0'; p1++, p2++, n--) {
		if (*p1 > *p2)
			return 1;
		else if (*p1 < *p2)
			return -1;
	}
	if (n == 0 || (*p1 == '\0' && *p2 == '\0'))
		return 0;
	else if (*p1 == '\0')
		return -1;
	else
		return 1;
}

int squeeze(char s1[], const char s2[]) {
	int count = 0, i = 0;
	while (s1[i] != '\0') {
		int j = 0;
		for (; s2[j] != '\0' && s1[i] != s2[j]; j++)
			;
		if (s2[j] == '\0') {
			i++;
		} else {
			int k = i + 1;
			for (; s1[k] != '\0'; k++) {
				s1[k - 1] = s1[k];
			}
			s1[k - 1] = '\0';
			count++;
		}
	}

	return count;
}

int find_replace_str(char str[], const char find_str[],
		const char replace_str[]) {
	int count = 0;
	int index = 0, find_len = strlen(find_str), replace_len = strlen(
			replace_str), offset = find_len - replace_len;

	while (strlen(str + index) >= find_len) {
		if (strncmp(str + index, find_str, find_len) == 0) {
			//把字符串剩余部分往后移-offset个位置
			if (offset < 0) {
				//剩余部分的字符个数＋1（'\0'）
				int n = strlen(str + index) - find_len + 1;
				for (int i = strlen(str); n > 0; i--, n--) {
					str[i + (-offset)] = str[i];
				}
			}
			//把字符串剩余部分往前移offset个位置
			else if (offset > 0) {
				//剩余部分的字符个数＋1（'\0'）
				int n = strlen(str + index) - find_len + 1;
				for (int i = index + find_len; n > 0; i++, n--) {
					str[i - offset] = str[i];
				}
			}
			//复制被替换成的串到str
			for (int i = 0; i < replace_len; i++) {
				str[index + i] = replace_str[i];
			}
			index += replace_len;
			count++;
		} else {
			index++;
		}
	}

	return count;
}

void map_fun(double d[], int n, double (*fp)(double d)) {
	for (int i = 0; i < n; i++) {
		d[i] = (*fp)(d[i]);
	}
}

void test_strlen() {
	char* s = "abdc";
	cout << strlen(s) << endl;
	cout << strlen2(s) << endl;
}

struct Node {
	int value;
	Node* next;
};

bool insert(Node* &h, int a, int pos) {
	Node *q;
	if (pos == 0) {
		q = new Node;
		q->value = a;
		q->next = h;
		h = q;
		return true;
	} else {
		Node *p = h;
		int i = 1;
		while (p != NULL && i < pos) {
			p = p->next;
			i++;
		}
		if (p != NULL) {
			q = new Node;
			q->value = a;
			q->next = p->next;
			p->next = q;
			return true;
		} else {
			return false;
		}
	}
}

bool remove(Node* &h, int &a, int pos) {
	Node *p = h, *q = NULL;
	int i = 1;
	while (p != NULL && i < pos) {
		q = p;
		p = p->next;
		i++;
	}
	if (p != NULL) {
		a = p->value;
		if (q != NULL) {
			q->next = p->next;
		} else {
			h = p->next;
		}
		delete p;
		return true;
	}
	return false;
}

//set
bool find(Node *h, int x) //在h中查找x
		{
	for (Node *p = h; p != NULL; p = p->next)
		if (p->value == x)
			return true;
	return false;
}
void insert(Node *&h, int x) //在h中增加一个元素
		{
	Node *p = new Node;
	p->value = x;
	p->next = h;
	h = p;
}
Node *input() //建立集合
{
	Node *h = NULL;
	int x;
	for (cin >> x; x != -1; cin >> x) {
		if (find(h, x))
			continue;
		insert(h, x);
	}
	return h;
}
Node *set_union(Node *h1, Node *h2) //集合“并”
		{
	Node *h = NULL, *p;
	//生成一个与h1一样的集合h
	for (p = h1; p != NULL; p = p->next)
		insert(h, p->value);
	//把h2加入到h中（去重）
	for (p = h2; p != NULL; p = p->next) {
		if (!find(h1, p->value))
			insert(h, p->value);
	}
	return h;
}
Node *set_intersection(Node *h1, Node *h2) //集合“交”
		{
	Node *h = NULL;
	for (Node *p = h1; p != NULL; p = p->next) {
		if (find(h2, p->value))
			insert(h, p->value);
	}
	return h;
}
Node *set_difference(Node *h1, Node *h2) //集合“差”
		{
	Node *h = NULL;
	for (Node *p = h1; p != NULL; p = p->next) {
		if (!find(h2, p->value))
			insert(h, p->value);
	}
	return h;
}
void output(Node *h) //输出集合的所有元素
		{
	for (Node *p = h; p != NULL; p = p->next)
		cout << p->value << ' ';
	cout << endl;
}
void remove(Node *&h) //删除集合
		{
	while (h != NULL) {
		Node *p = h;
		h = h->next;
		delete p;
	}
}
int test_set() {
	Node *set1, *set2, *set3, *set4, *set5;

	set1 = input();
	set2 = input();
	set3 = set_union(set1, set2);
	set4 = set_intersection(set1, set2);
	set5 = set_difference(set1, set2);

	output(set1);
	output(set2);
	output(set3);
	output(set4);
	output(set5);

	remove(set1);
	remove(set2);
	remove(set3);
	remove(set4);
	remove(set5);

	return 0;
}

//set

void insert_sort(Node *&h) {
	if (h == NULL)
		return;

	Node *q = h->next; //q指向B部分的第一个元素
	h->next = NULL; //h指向A部分的第一个元素（初始状态下，A部分只有一个元素）
	while (q != NULL) { //对第二部分的元素进行循环

		//从B部分取一个元素
		Node *q1 = q; //q1指向取到的元素
		q = q->next; //从B中去掉一个元素

		//循环在A部分中从头开始找一个元素（p指向它），使得（q1->content）小于（p->content）
		Node *p = h, *p1 = NULL; //p指向A部分第一个元素；p1指向p指向的前一个结点，初始化为空
		while (p != NULL && q1->value > p->value) {
			p1 = p;
			p = p->next;
		}
		if (p1 != NULL) {
			q1->next = p;
			p1->next = q1;
		} else {
			q1->next = h;
			h = q1;
		}
	}
}

//eight queen

const int N = 8;
int count = 0;

void eight_queen_show(int *a) {
	count ++;
	cout << "NO." << count << " : " << endl;;
	for (int i = 0; i < N; i ++) {
		for (int j = 0; j < N; j ++) {
			if (j == a[i]) {
				cout << "●";
			} else {
				cout << "○";
			}
		}
		cout << endl;
	}
}

bool eight_queen_check(int *a, int n) {
	//check the lines before n
	for (int i = 0; i < n; i ++) {
		//same col
		if (a[i] == a[n]) {
			return false;
		}
		//same dial
		if (abs(n - i) == abs(a[n] - a[i])) {
			return false;
		}
	}
	return true;
}

void eight_queen_put(int *a, int n) {
	if (n >= N) {
		return;
	}
	//put the n's queen at (n, a[j])
	for (int j = 0; j < N; j ++) {
		a[n] = j;
		if (eight_queen_check(a, n)) {
			if(n == N - 1) {
				eight_queen_show(a);
				return;
			}
			else {
				eight_queen_put(a, n + 1);
			}
		}
	}
}

void eight_queen_init() {
	int a[N] = {0};
	eight_queen_put(a, 0);
}

int main(int argc, char** argv) {
//	test_hermit();
//	test_ack();
//	test_strlen();
	eight_queen_init();

	return 0;
}

