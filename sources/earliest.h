#pragma once
#include "map.h"
class Earliest {
public:
	Clock start,arrive;  // start 即计时起点  arrive即到达时间
	int src, dest; //src : source city  dest: destination city
	Way path[11]; // 这个path是从dest到src的
	int pathlen = 0; // 这是path长度  
	void compute(Map &m);
};

typedef struct city {
	bool decided=false;
	int pre_city=-1;
	Way pre_way;
	Clock arrive;
}cc;