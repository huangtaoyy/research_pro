//#include<iostream>
//#include <stdio.h>
//using namespace std;
////abcdefghijklmnopqrstuvwxyzabcabcaba
//
//void print_array(int *b, int size) {
//	for (int i = 0; i < size; i ++) {
//		printf("%d", b[i]);
//	}
//	printf("\n");
//}
//
//int main(){
//	char a[500] = {0};
//	int b[26] = { 0 };
//
//	int max1 = 0, max2 = 0;
//
//	cin.getline(a, 500, '\n');
//	int i = -1;
//	while (a[++ i])
//		a[i] = toupper(a[i]);
//
//	//ͳ����ĸ���ִ���
//	for (int i = 0; i < 500; i++){
//		if (a[i]>=65 && a[i]<=90){
//			b[a[i] - 65]++;
//		}
//	}
//
//	//�ҳ����ִ�������
//	for (int i = 0; i < 26; i++){
//		if (max1<b[i])
//		    max1 = b[i];
//	}
//	//�����ִ�������ȫ���[Ϊ0
//	for (int i = 0; i < 26; i++){
//		if (b[i]==max1){
//			b[i] = 0;
//		}
//	}
//	//���ִ���ڶ������ĸ�ʹ���
//	for (int i = 0; i < 26; i++){
//		if (max2<b[i]){
//			max2 = b[i];
//		}
//	}
//	//��һ�γ���max2�ε���ĸ
//	for (int i = 0; i < 500; i++){
//		if (a[i] >= 65 && a[i] <= 90 && b[a[i] - 65]==max2){
//			cout << a[i] << '+' << char(a[i] + 32) << ':' << max2 << endl;
//			break;
//		}
//	}
//	return 0;
//}

#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	string s("WelcomeToWebSite!");
	cout<<s<<endl;transform(s.begin(),s.end(),s.begin(),::tolower);//字母转小写
	cout<<s<<endl;transform(s.begin(),s.end(),s.begin(),::toupper);//字母转大写
	cout<<s<<endl;

	return 0;
}
