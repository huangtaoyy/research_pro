///*
// * main.cpp
// *
// *  Created on: 2014-5-17
// *      Author: huangtao
// */
//#include <iostream>
//
//using namespace std;
//
//int main(int argc, char* argv[]) {
//	int var = 33;
//	try {
//		var = 44;
//		throw 3;
//	} catch(int e) {
//		cout << "exception caught!" << endl;
//	}
//}

#include <iostream>

using namespace std;

class Solution {
public:
	void reverse(string &t) {
		int length = t.length();
		if(length <= 0) return;

		int i = 0;
		for (; i < length / 2; i ++) {
			char temp = t[i];
			t[i] = t[length - 1 - i];
			t[length - 1 - i] = temp;
		}
	}

	void reverseWords(string &s) {
		string rs;
		for (int i = s.length() - 1; i >= 0;) {
			while (i >= 0 && s[i] == ' ')
				i--;
			if (i < 0) break;
			if (!rs.empty())
				rs.push_back(' ');

			string t;
			while (i >= 0 && s[i] != ' ')
				t.push_back(s[i--]);

			reverse(t);
			rs.append(t);
		}
		s = rs;
	}
};
//
//int main() {
//	string s = " the  sky  is  blue ";
//	cout << s << endl;
//	Solution so;
//	so.reverseWords(s);
////	so.reverse(s);
//	cout << s << endl;
//	return 0;
//}
