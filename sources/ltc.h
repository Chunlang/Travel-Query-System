#pragma once
#include "mincost.h"
#include "mintime.h"

// name of this header means "limited time cheapest"  
typedef struct NextPass {
	int city;
	int t_d;
	int c_d;
}nextP;

class LTC {
public:
	int src, dest;
	Clock t0;
	int t_limit;
	int maxcost;
	int mincost;
	int pass[3];
	int pathlen=0;
	Way path[44];
	LTC() {
		maxcost = 1000000;
		mincost = 1000000;
		pathlen = 0;
	}
	void compute0(Map &); //�ƺ�Ҫ���� t0�Խ����Ӱ��   
	// ��һ��·�������ѡ  �ڿ��г�����ѡ �ڿ���·����ѡ ��̭������ͬһ������ �Ⱥ�ʱ���ֵ������ֹ� �İ��  
	// ��ͬ�����ڵ�һ���ƺ����ܱȽ� 
	// ȷ���õ�һ��֮�󣬺����й�ʱ��ļ���Ͷ��� ����ʱ��������  
	// �ؼ���ÿ�ζ�Ҫ��¼��·  ������Ҫ�����ṹ�� �洢��·  ������������   here, ��binary tree��ʾgeneral tree
	void computeN(Map &, int);
	bool DFS(int i, Way* pathCpy, Map &m, nextP* nextpass);
};

typedef struct Stat {
	bool candidate = true;
	int t_inf;  // time �½�
	int c_inf; // cost �½�
	int t_d; // least time from here to dest;
	int c_d; // least cost from here to dest;
}sta;

class routeNode {
public:
	bool candidate[numCities];
	Clock arrive;
	int time_sum;
	int cost_sum;
	int city;
	Way pre_way;
	routeNode* pre_node;
	routeNode* pre_sis = NULL;
	routeNode* child; //left    if(child==NULL) this node is a leaf;  
	routeNode* sister; //right

	void init() {
		for(int i=0;i<numCities;i++)
			candidate[i] = true;
		time_sum = 0; cost_sum = 0;
		pre_node = NULL;
		child = NULL;
		sister = NULL;
	}
	int addNode(Way w,int tl,int mc,sta* stn) {
		routeNode* kid = new routeNode;  // �ǵ�delete
		kid->init();
		kid->city = w.there;
		for (int i = 0; i < numCities; i++)
			kid->candidate[i] = candidate[i];
		kid->candidate[kid->city] = false;
		kid->arrive = arrive;
		kid->arrive.add(w.go_time, w.get_time);
		kid->cost_sum = cost_sum + w.cost;
		kid->time_sum = this->time_sum + kid->arrive.hour - this->arrive.hour + (kid->arrive.day - this->arrive.day) * 24;
		if (kid->time_sum + stn[kid->city].t_d > tl || kid->cost_sum + stn[kid->city].c_d > mc) {
			delete kid;
			kid = NULL;
			return mc;
		}
		kid->pre_node = this;
		kid->pre_way = w;
		// link this and the kid :
		if (!child) {
			child = kid;
		}
		else {
			routeNode* daughter1=child;
			routeNode* daughter2 = child;
			while (daughter2) {
				daughter1 = daughter2;
				daughter2 = daughter2->sister;
			}
			daughter1->sister = kid;
			kid->pre_sis = daughter1;
		}
		if (kid->cost_sum < mc) return kid->cost_sum;
		return mc;
	}
	void delNode() {
		if (pre_sis == NULL) {
			pre_node->child = sister;
			if (sister)
				sister->pre_sis = NULL;
			delete this;
		}
		else {
			pre_sis->sister = sister;
			if (sister)
				sister->pre_sis = pre_sis;
			delete this;
		}
	}
	bool traverAdd(int depth,int dest,int &maxcost,Map &m,int tl,sta* stn) {
		bool ret = true;  // ret=true ���� ����Ҷ�Ӷ���dest ���ܼ���ǰ����
		if(sister) ret=sister->traverAdd(depth,dest,maxcost,m,tl,stn);
		if (!child) {
			if((city==dest&&cost_sum>maxcost)||(city!=dest&&depth>0)) {
				delNode();
				return ret;
			}
			if (city == dest) return ret;
			{
				City* adj = m.cities[city];
				while (adj) {
					if (candidate[adj->city]) {
						Way* w = adj->ways;
						if (adj->city != dest) {
							while (w) {
								addNode(*w, tl, maxcost, stn); //ֻ�����ѵ���destʱ��ʹ�� maxcost=...
								w = w->next;
							}
						}
						else {
							while (w) {
								maxcost=addNode(*w, tl, maxcost, stn); //ֻ�����ѵ���destʱ��ʹ�� maxcost=...
								w = w->next;
							}
						}
					}
					adj = adj->next;
				}
				
				return false;
			}
		}
		return ret&child->traverAdd(depth - 1,dest,maxcost,m,tl,stn);
	}

	routeNode* traverFind(int dest) {
		if (city == dest) return this;
		routeNode* ret = NULL;
		if(child)
			ret = child->traverFind(dest);
		if (ret) return ret;
		if (sister)
			ret = sister->traverFind(dest);
		return ret;
	}
};

// ����Ϊ���䲿�֣�





