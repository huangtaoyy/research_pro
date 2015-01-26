/*
 * njie.cpp
 *
 *  Created on: 2014-6-5
 *      Author: huangtao
 */
#include <iostream>
using namespace std;

//n!
int formula(int n) {
	if (n <= 1)
		return n;
	return n * formula(n - 1);
}

int formula(int m, int n) {
	int count = 0;
	count = formula(m + n) / (formula(m) * formula(n));
	return count;
}

//k = n * 1 + m * 2
int frog(int k) {
	if (k <= 1)
		return k;

	int count = 0;
	int n = 0, m = 0;
	for (int i = 0; i <= k; i++) {
		n = i;
		if ((k - n) % 2 == 0 && k - n != 0) {
			m = (k - n) / 2;
			cout << "1   n = " << n << ", m = " << m << endl;
			if (n == 0) {
				count++;
			} else {
				count = count + formula(m, n);
			}
		} else if (k - n > 0 && k - n < 2) {
			m = (k - n) / 2;
			cout << "2   n = " << k << ", m = " << m << endl;
			count ++;
		}
	}
	return count;
}

int f(int n) {
	if (n <= 2)
		return n;
	return f(n - 2) + f(n - 1);
}

//int main() {
//	int n = 14;
//	int count = frog(n);
//	cout << count << endl;
//	cout << f(n) << endl;
//	return 0;
//}

