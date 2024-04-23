#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <ctime>
#include <random>

using namespace std;
typedef struct point {//�㼯
	int id;           //���id
	double x, y;      //���λ��
	int neigid;       //������ͨ�ĵ�
	int poi;          //���
	double score;        //����
	point() {
		x = -1;
	}
}point;
vector<pair<point, vector<int>>> graph;
void insertFun(pair<point, vector<int>>& temp, int inser) {
	for (int i = 0; i < temp.second.size(); i++) {
		if (temp.second[i] == inser)
			return;
	}
	temp.second.push_back(inser);
}
int main() {
	for (int i = 0; i < 50; i++) {
		graph.push_back(make_pair(point(), vector<int>()));
	}
	fstream File("D:\\����ļ�\\Visual Studio\\ROSE\\map\\ds.node", fstream::out | ios_base::trunc);
	if (File.is_open()) {
		cout << "�ļ��򿪳ɹ�" << endl;
		default_random_engine ran_num1, ran_num2, ran_num3, ran_num4, ran_num5;
		uniform_int_distribution<int> scopeDouble(0, 1000);
		uniform_int_distribution<int> scopeDouble1(0, 100);
		uniform_int_distribution<int> scopeInt(1, 10);
		uniform_int_distribution<int> circulationLen(1, 5);
		uniform_int_distribution<int> neigPoi(0, 49);
		ran_num1.seed(time(0));
		ran_num2.seed(time(0));
		ran_num3.seed(time(0));
		ran_num4.seed(time(0));
		ran_num5.seed(time(0));
		for (int i = 0; i < 50; i++) {
			if (graph[i].first.x == -1) {
				graph[i].first.x = (double)scopeDouble(ran_num1) / 10;
				graph[i].first.y = (double)scopeDouble(ran_num1) / 10;
				graph[i].first.poi = scopeInt(ran_num2);
				graph[i].first.score = (double)scopeDouble1(ran_num5) / 10;
			}
			int len = circulationLen(ran_num3);
			for (int n = 0; n < len; n++) {
				int inse = neigPoi(ran_num4);
				if (inse == i)
					continue;
				else if (graph[i].second.size() < 5 && graph[inse].second.size() < 5) {
					insertFun(graph[i], inse);
					insertFun(graph[inse], i);
				}
			}
		}
		for (int i = 0; i < 50; i++) {
			File << i << " " << graph[i].first.x
				<< " " << graph[i].first.y
				<< " " << graph[i].first.poi
				<< " " << graph[i].first.score;
			/*		for (int n = 0; n < graph[i].second.size(); n++) {
						File <<" "<< graph[i].second[n];
					}*/
			File << endl;
		}
		File.close();
		cout << "���" << endl;
	}
	else {
		cout << "�ļ���ʧ��" << endl;
	}
	return 0;
}