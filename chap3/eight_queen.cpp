///*
// * eight_queen.cpp
// *
// *  Created on: 2015-1-21
// *      Author: huangtao
// */
//
//#include <iostream>
//using namespace std;
//
//bool a[8]; //a[i]表示第i行是否可以放皇后
//bool b[15]; //b[k]表示“从左下往右上”('/')的第k个对角线是否可以放皇后
//bool c[15]; //c[k]表示“从左上往右下”('\')的第k个对角线是否可以放皇后
////与棋盘第i行、第j列的格子所对应的行和对角线为：a[i],b[i+j],c[j-i+7]
//
//int x[8]; //x[j]表示第j列上皇后的位置（所在的行）。
//
//bool try_by_col(int j) {
//	for (int i = 0; i < 8; i++) //选择第j列中可放皇后的行，然后递归选择第j+1列可放皇后的行...。
//			{
//		if (a[i] && b[i + j] && c[j - i + 7]) //第j列第i行的位置所在的行以及两个对角线上无皇后。
//				{
//			x[j] = i; //设置第j列皇后的位置。
//			a[i] = b[i + j] = c[j - i + 7] = false; //把第j列皇后所在的行以及两个对角线设为已占用。
//			if (j == 7 || try_by_col(j + 1)) //是最后一列或第j+1列皇后位置选择成功。
//				return true;
//			else
//				//第j+1列皇后位置选择失败。
//				a[i] = b[i + j] = c[j - i + 7] = true; //取消第j列皇后位置，准备选择下一个位置。
//		}
//	}
//	return false;
//}
//
//int eight_queen() {
//	int i, j, k;
//	//初始化：所有行以及对角线可放皇后。
//	for (i = 0; i < 8; i++)
//		a[i] = true;
//	for (k = 0; k < 15; k++)
//		b[k] = true;
//	for (k = 0; k < 15; k++)
//		c[k] = true;
//
//	if (try_by_col(0)) //从第0列开始尝试放皇后的位置。
//			{ //x[0],x[1],...,x[7]分别为第一列、第二列、...、第七列上皇后的位置（所在的行）
//		for (j = 0; j < 8; j++) //按列输出皇后
//				{
//			for (i = 0; i < x[j]; i++)
//				cout << "|_";
//			cout << "|Q|";
//			for (i = x[j] + 1; i < 8; i++)
//				cout << "_|";
//			cout << endl;
//		}
//	}
//	cout << endl;
//	return 0;
//}
//
//int main(int argc, char** argv) {
//	eight_queen();
//
//	return 0;
//}
//

/*
 * Copyright (c) leo
 * All rights reserved.
 * filename: nQueens
 * summary :
 * version : 1.0
 * author : leo
 * date : 8.12.2011
 *问题：
 *    在n*n (n=1 or n>=4 )的棋盘上放置n个皇后，如果在同一行，同一列，同一对角线上都不存在两个皇后，
 *    那么这个棋盘格局就是n皇后的一个解。
 *要求：
 *    找出n皇后的一组解即可，打印出放置满足n皇后条件的棋子位置
 */
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define N 8 //皇后数=棋盘行列数
int a[N]; //a[i]为第i行皇后所在列
int t = 1;

void show() //图形化输出
{
	int i;
	int p, q;
	int b[N][N] = { {0} };

	int mq = 0;
	printf("第%d个解为: ", t++);
	for (i = 0; i < N; i++) {
		b[i][a[i]] = 1;
		printf("(%d,%d) ", i, a[i]);
	}
	printf("\n");
	for (p = 0; p < N; p++) {
		for (q = 0; q < N; q++) {
			if (b[p][q] == 1)
				printf("●");
			else
				printf("○");
		}
		printf("\n");
	}
}
int check(int n) { //满足条件返回1，否则返回0

	int i;
	for (i = 0; i < n; i++) {
		if (a[i] == a[n] || abs(n - i) == abs(a[i] - a[n])) //at the same column or diagonal (对角线)
			return 0;
	}
	return 1;
}
void put(int n) //在第n行放置第n个皇后
		{
	int i;
	if (n == N)
		return;
	for (i = 0; i < N; i++) {
		a[n] = i;
		if (check(n)) { //位置合法
			if (n == N - 1) { //皇后全部放置完毕
				show();
			} else {
				put(n + 1);
			}
		}
	}
}
//int main() {
//	put(0);
//	return 0;
//}
