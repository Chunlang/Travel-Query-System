#pragma once
#include "map.h"
class Earliest {
public:
	Clock start,arrive;  // start ����ʱ���  arrive������ʱ��
	int src, dest; //src : source city  dest: destination city
	Way path[11]; // ���path�Ǵ�dest��src��
	int pathlen = 0; // ����path����  
	void compute(Map &m);
};

typedef struct city {
	bool decided=false;
	int pre_city=-1;
	Way pre_way;
	Clock arrive;
}cc;