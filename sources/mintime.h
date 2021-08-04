#pragma once
#include "earliest.h"

//  src A B C dest      Clock t0，t1:出发时间不得早于t0   不得晚于t1 其中，t1<t0+1天   返回全局最小耗时  
//  假设总是有解  
//  pass-by : 0或1或2或3   
class Mintime {
public:
	int src, dest;
	int pass[3];
	int pathlen;
	Way path[44];
	int mintime;
	// pass=0;   return mintime and pathway  
	Clock t0, t1;
	Mintime() {
		pathlen = 0;
		mintime = 100000;
	}
	void init() {
		t0.day = 0; t0.hour = 0;
		t1.day = 0; t1.hour = 23;
	};
	void compute0(Map &m);
	void computeN(Map &m, int n);
};