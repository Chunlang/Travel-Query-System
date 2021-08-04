#include "stdafx.h"
#include "mintime.h"

void Mintime::compute0(Map &m) {
	//t0--t1  
	Clock t = t0, t_best; // t_best 代表选中的那个
	Earliest earliest; earliest.src = src; earliest.dest = dest;
	mintime = 100000;
	while (t.day != t1.day || t.hour != t1.hour) {
		earliest.start = t;
		earliest.compute(m);  // t为最早出发时间条件下 计算
		// 如果有两个不同初始时刻计算出来的线路是完全一样的，那arrive时间必一样，则duration必不同
		int duration = earliest.arrive.hour - earliest.start.hour + (earliest.arrive.day - earliest.start.day) * 24;
		if (duration < mintime) {
			mintime = duration;
			t_best = t;
		}
		t.inc();
	}
	earliest.start = t_best;
	earliest.compute(m); // 每个线路都存储太耗空间，所以仅保留t_best信息，用compute恢复完整线路信息
	for (int i = earliest.pathlen - 1; i >= 0; i--) {
		path[earliest.pathlen - 1 - i] = earliest.path[i];  // Mintime.path 是按照从src到dest的正常顺序的
	}
	pathlen = earliest.pathlen;
}


// 按照循环来把  统一按照 3个 pass-by的情形 讨论  
// n pass-by cities  
void Mintime::computeN(Map &m, int n) {
	mintime = 100000;
	Fact f;
	int all = f.fact(n);
	int j = 0;
	int passBest[3];
	Clock tBest;
	int tMin = 100000;
	
	for (int i = 0; i<n; i++) {
		passBest[i] = pass[i];
	}
	//123  132  312 321 231 213
	// complete pailie
	for (int i = 0; i<all; i++) {
		//compute a time-saving route for a particular city-pailie:
		Clock t = t0, t_best; 
		Earliest earliest;
		tMin = 100000;
		while (t.day != t1.day || t.hour != t1.hour) {
			earliest.start = t;
			earliest.src = src;
			for (int cnt = 0; cnt <= n - 1; cnt++) {
				earliest.dest = pass[cnt];
				earliest.compute(m);
				earliest.src = earliest.dest; // src ,dest are places
				earliest.start = earliest.arrive; // start, arrive are clocks
			}
			earliest.dest = dest;
			earliest.compute(m);

			int duration = earliest.arrive.hour - t.hour + (earliest.arrive.day - t.day) * 24;
			if (duration < tMin) {
				tMin = duration;
				t_best = t;
			}
			t.inc();
		}
		if (tMin < mintime) { // 保存当前最优
			mintime = tMin;
			tBest = t_best;
			for (int ii = 0; ii < n; ii++)
				passBest[ii] = pass[ii];

		}
		// change city-pailie:
		{
			int temp = pass[j];
			pass[j] = pass[(j + 1) % n];
			pass[(j + 1) % n] = temp;
			j++;
			j %= n;
		}
	}
	
	// 用计算恢复 线路信息：
	pathlen = 0;
	Earliest earliest;
	earliest.start = tBest;
	earliest.src = src;
	for (int cnt = 0; cnt < n; cnt++) {
		earliest.dest = passBest[cnt];
		earliest.compute(m);
		{
			for (int i = 0; i < earliest.pathlen ; i++) {
				path[pathlen+i] = earliest.path[earliest.pathlen-1-i];  // Mintime.path 是按照从src到dest的正常顺序的
			}
			pathlen += earliest.pathlen;
		}
		earliest.src = earliest.dest; // src ,dest are places
		earliest.start = earliest.arrive; // start, arrive are clocks
	}
	earliest.dest = dest;
	earliest.compute(m);
	{
		for (int i = 0; i < earliest.pathlen; i++) {
			path[pathlen + i] = earliest.path[earliest.pathlen - 1 - i];  // Mintime.path 是按照从src到dest的正常顺序的
		}
		pathlen += earliest.pathlen;
	}// 至此完成 完整线路的计算 

}
