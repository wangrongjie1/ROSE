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

	string File = "D:\\����ļ�\\Visual Studio\\ROSE\\map\\ds.node";
	initializeTable(File);//��ʼ�����ݽṹpoi,rote
	initializeOS();//��ʼ�أ�Ŀ�ĵأ�;���ĵ�����ֵ,�޳����ϸ�poi�㣬������ֲ�������Ĭ��λƽ����
	seekReferenceNode();//Ѱ�Ҳο��ڵ�
	calculateLB();//����ÿ��poi�ڵ���ο��ڵ�ľ���
	ROSE();//ʵ��ROSE�㷨
	cout << OS[OS.size() - 1][0].first << ":";
	for (auto i : OS[OS.size() - 1][0].second) {
		cout << i << " ";
	}
	return 0;
}