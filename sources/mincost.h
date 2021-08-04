#pragma once
#include "map.h"

class Mincost{
public:
	int src, dest;
	int pass[3];
	int pathlen;
	Way path[44];  // �����path�Ǵ���src��dest������˳��  
	
	int mincost;
	Mincost() {
		pathlen = 0;
		mincost = 1000000;
	}
	void init();
	void compute0(Map &m);
	void computeN(Map &m, int n);
};

typedef struct Station {
	int decided;
	int cost;
	Way pre_way;
}station;