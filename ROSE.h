#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include <math.h>
#include<bitset>
#include<map>
using namespace std;
#ifndef ROSE_H
#define ROSE_H
namespace rose {
#define eucDis(a,b) (((a)-(b))*((a)-(b)))//欧式几德距离
#define keyword 7//点类别的总数
#define RNcount 10//参考节点的总数
	typedef struct point {//点集
		int id;           //点的id
		double x, y;      //点的位置
		int poi;          //类别
		double score;        //评分
		vector<int> neigid;//与其相通的点
	}point;
	vector<pair<int, double>> CT;//输入的信息
	vector<point> rote;   //路网
	vector<vector<int>> poi;//节点的类别
	vector<double> avgScore;//相同POI的平均评分
	vector<int> RN;//参考节点
	map<int, vector<double>> disRN;//目标节点到参考节点的距离
	map<int, map<int, double>> LB;//poi节点之间的距离
	vector < vector<pair<double,vector<int>>>> OS;//ROSE算法的动态规划矩阵

	double swaptype(char* p) {
		return stod(p);
	}
	//初始化数据结构poi,rote
	bool initializeTable(string filePath) {
		ifstream file(filePath);
		if (!file.is_open())
			return false;
		string temp;
		char arr[100];
		while (getline(file, temp)) {
			point temppoint;
			strcpy(arr, temp.c_str());
			temppoint.id = (int)swaptype(strtok(arr, " "));
			temppoint.x = swaptype(strtok(NULL, " "));
			temppoint.y = swaptype(strtok(NULL, " "));
			temppoint.poi = (int)swaptype(strtok(NULL, " "));
			temppoint.score = swaptype(strtok(NULL, " "));
			char* p;
			for (int i = 0; p = strtok(NULL, ","); i++) {
				temppoint.neigid.push_back(stoi(p));
			}
			rote.push_back(temppoint);
		}
		//初始化poi和avgscore;
		for (int i = 0; i < keyword; i++) {
			poi.push_back(vector<int>());
			avgScore.push_back(0);
		}
		for (int i = 0; i < rote.size(); i++) {
			poi[rote[i].poi].push_back(rote[i].id);
			avgScore[rote[i].poi] += rote[i].score;
		}
		for (int i = 1; i < avgScore.size(); i++) {
			avgScore[i] /= poi[i].size();
		}
	}

	//起始地，目的地，途径的点与阈值,剔除不合格poi点，如果评分不输入则默认位平均数
	void initializeOS() {
		OS.resize(CT.size());
		OS[0].push_back(make_pair(0, vector<int>{CT[0].first}));
		for (int i = 1; i < CT.size()-1; ++i) {
			OS[i].resize(poi[CT[i].first].size(), make_pair(0, vector<int>()));
		}
		OS[CT.size()-1].push_back(make_pair(0, vector<int>()));
		for (int i = 1; i < CT.size() - 1; ++i) {
			for (int n = 0; n < poi[CT[i].first].size(); ++n) {
				if (rote[poi[CT[i].first][n]].score < CT[i].second) {
					poi[CT[i].first].erase(poi[CT[i].first].begin() + n);
					--n;
				}
			}
		}
	}

	//返回距离和覆盖节点总数。
	vector < vector<int>> shortestPath(int vs) {
		vector<bool> sing(rote.size(), 0);
		vector < vector<int>> passNode;
		vector < vector<int>> rePassNode;
		vector<double> distance = { 0 };
		passNode.push_back(vector<int>());
		passNode[0].push_back(vs);
		sing[vs] = 1;
		while (1) {
			int id = *(passNode[0].end() - 1);
			bool flag = 1;
			for (int i = 0; i < rote[id].neigid.size(); i++) {
				if (sing[rote[id].neigid[i]])
					continue;
				flag = 0;
				sing[rote[id].neigid[i]] = 1;
				double tempdis = distance[0] + sqrt(eucDis(rote[id].x, rote[rote[id].neigid[i]].x)
					+ eucDis(rote[id].y, rote[rote[id].neigid[i]].y));
				for (int n = 0; (distance.end() - n) != distance.begin(); ++n) {
					if (tempdis > distance[distance.size() - n - 1]) {
						distance.insert(distance.end() - n, tempdis);
						passNode.insert(passNode.end() - n, passNode[0]);
						passNode[passNode.size() - n - 1].push_back(rote[id].neigid[i]);
						break;
					}
				}
			}
			if (flag)
				rePassNode.push_back(passNode[0]);
			passNode.erase(passNode.begin());
			distance.erase(distance.begin());
			if (!distance.size())
				return rePassNode;
		}
	}

	//寻找参考节点
	void seekReferenceNode() {
		vector<int> singNode(poi[0].size(), 0);
		for (int i = 0; i < singNode.size(); i++) {
			vector < vector<int>> passNode = shortestPath(poi[0][i]);
			int twainCount = 0;
			for (int n = 0; n < passNode.size(); n++) {
				int poiCount = 0;
				for (int l = 0; l < passNode[n].size(); l++) {
					if (rote[passNode[n][l]].poi)
						++poiCount;
				}
				twainCount +=poiCount*(poiCount+1)/2;
			}
			singNode[i] = twainCount;
		}
		for (int i = 0; i < RNcount; ++i) {//参考节点一定小于普通节点总数
			int maxSingNode = singNode[0], maxLocation = 0;
			for (int n = 1; n < singNode.size(); ++n) {
				if (singNode[n] > maxSingNode) {
					maxSingNode = singNode[n];
					maxLocation = n;
				}
			}
			RN.push_back(poi[0][maxLocation]);
			singNode.erase(singNode.begin() + maxLocation);
		}
	}

	//求两点间实际距离
	double shortestPath(int vs,int vd) {
		if (vs == vd)
			return 0;
		vector<bool> sing(rote.size(), 0);
		vector<int> passNode;
		vector<double> distance = { 0 };
		passNode.push_back(vs);
		sing[vs] = 1;
		while (1) {
			int id = passNode[0];
			for (int i = 0; i < rote[id].neigid.size(); ++i) {
				if (sing[rote[id].neigid[i]])
					continue;
				sing[rote[id].neigid[i]] = 1;
				double tempdis = distance[0] + sqrt(eucDis(rote[id].x, rote[rote[id].neigid[i]].x)
					+ eucDis(rote[id].y, rote[rote[id].neigid[i]].y));
				if (rote[id].neigid[i] == vd)//到达终点
					return tempdis;
				for (int n = 0; (distance.end() - n) != distance.begin(); ++n) {
					if (tempdis > distance[distance.size() - n - 1]) {
						distance.insert(distance.end() - n, tempdis);
						passNode.insert(passNode.end() - n, rote[id].neigid[i]);
						break;
					}
				}
			}
			passNode.erase(passNode.begin());
			distance.erase(distance.begin());
		}
	}
	
	//计算每个poi节点与参考节点的距离
	void calculateLB() {
		for (int i = 1; i < poi.size(); ++i) {
			for (int n = 0; n < poi[i].size(); ++n) {
				for (int l = 0; l < RNcount; ++l)
					disRN[poi[i][n]].push_back(shortestPath(poi[i][n], RN[l]));
			}
		}
		if (disRN.find(CT[0].first) == disRN.end()) {
			for (int l = 0; l < RNcount; ++l)
				disRN[CT[0].first].push_back(shortestPath(CT[0].first, RN[l]));
		}
		if (disRN.find(CT[CT.size()-1].first) == disRN.end()) {
			for (int l = 0; l < RNcount; ++l)
				disRN[CT[CT.size()-1].first].push_back(shortestPath(CT[CT.size()-1].first, RN[l]));
		}
		for (auto it = disRN.begin(); it != disRN.end(); ++it) {
			for (auto i = it; i != disRN.end(); ++i) {
				double maxReDis = fabs(disRN[it->first][0]-disRN[i->first][0]);
				for (int n = 1; n < RNcount; ++n) {
					double tempReDis = fabs(disRN[it->first][n] - disRN[i->first][n]);
					if (tempReDis > maxReDis)
						maxReDis = tempReDis;
				}
				LB[it->first][i->first] = maxReDis;
				LB[i->first][it->first] = maxReDis;
			}
		}
	}

	void calculateOSi(int vs,int i,int n) {
		double minDis = LB[vs][OS[i - 1][0].second[OS[i - 1][0].second.size() - 1]] + OS[i - 1][0].first;
		int passNodeId = 0;
		for (int h = 1; h < OS[i - 1].size(); ++h) {
			double disTemp = LB[vs][OS[i - 1][h].second[OS[i - 1][h].second.size() - 1]] + OS[i - 1][h].first;
			if (minDis > disTemp) {
				minDis = disTemp;
				passNodeId = h;
			}
		}
		OS[i][n].first = minDis;
		OS[i][n].second = OS[i - 1][passNodeId].second;
		OS[i][n].second.push_back(vs);
	}

	void ROSE() {
		while (1) {
			for (int i = 1; i < CT.size()-1; ++i) {
				for (int n = 0; n < poi[CT[i].first].size(); ++n) {
					calculateOSi(poi[CT[i].first][n],i, n);
				}
			}
			calculateOSi(CT[CT.size() - 1].first, CT.size() - 1, 0);
			double practicalDis = 0;
			for (int i = 1; i < OS[OS.size() - 1][0].second.size(); ++i) {
				LB[OS[OS.size() - 1][0].second[i]][OS[OS.size() - 1][0].second[i-1]]= shortestPath(OS[OS.size() - 1][0].second[i], OS[OS.size() - 1][0].second[i - 1]);
				LB[OS[OS.size() - 1][0].second[i - 1]][OS[OS.size() - 1][0].second[i]] = LB[OS[OS.size() - 1][0].second[i]][OS[OS.size() - 1][0].second[i - 1]];
				practicalDis += LB[OS[OS.size() - 1][0].second[i - 1]][OS[OS.size() - 1][0].second[i]];
			}
			if (practicalDis == OS[OS.size() - 1][0].first)
				return;
		}
	}
}
#endif 

