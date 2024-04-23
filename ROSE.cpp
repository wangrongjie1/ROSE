#define  _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include"ROSE.h"
using namespace std;
using namespace rose;

int main() {
	int arr[] = { 31,27 };
	vector<int> C = { 4,3,2,1,1 };
	vector<double> T = { 0,0,0,0,0 };
	CT.push_back(make_pair(arr[0], 0));
	for (int i = 0; i <C.size() ; i++) {
		CT.push_back(make_pair(C[i], T[i]));
	}
	CT.push_back(make_pair(arr[1], 0));

	string File = "D:\\编程文件\\Visual Studio\\ROSE\\map\\ds.node";
	initializeTable(File);//初始化数据结构poi,rote
	initializeOS();//起始地，目的地，途径的点与阈值,剔除不合格poi点，如果评分不输入则默认位平均数
	seekReferenceNode();//寻找参考节点
	calculateLB();//计算每个poi节点与参考节点的距离
	ROSE();//实现ROSE算法
	cout << OS[OS.size() - 1][0].first << ":";
	for (auto i : OS[OS.size() - 1][0].second) {
		cout << i << " ";
	}
	return 0;
}