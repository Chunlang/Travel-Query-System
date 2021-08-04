#include "stdafx.h"
#include "mincost.h"

void Mincost::compute0(Map &m) {
	mincost = 1000000;
	station stn[numCities];
	for (int i = 0; i<numCities; i++) {
		stn[i].decided = 0;
		stn[i].cost = -1;
	}
	stn[src].decided = 1;  // decided  
	stn[src].cost = 0;
	// find adjacent nodes 
	City* adj = m.cities[src];  //  cong a certain city start  its connected cities 
	int pre = src;
	while (pre != dest) {
		while (adj) {
			if (stn[adj->city].cost == -1) {
				stn[adj->city].cost = stn[pre].cost + adj->cheap(); //cheap(adj->ways)是从pre-->adj的最小费用
				stn[adj->city].pre_way = adj->w_cheap;
			}
			else { 
				//stn[adj->city].cost = min(stn[pre].cost + adj->cheap(), stn[adj->city].cost); 
				int temp = stn[pre].cost + adj->cheap();
				if (temp < stn[adj->city].cost) {
					stn[adj->city].cost = temp;
					stn[adj->city].pre_way = adj->w_cheap;
				}
			}
			adj = adj->next;
		}
		int minlocal = 1000000, j = 0;
		for (int i = 0; i<numCities; i++) {
			if (stn[i].decided == 0) {
				if (minlocal>stn[i].cost) {
					minlocal = stn[i].cost;
					j = i;
				}
			}
		}

		pre = j;
		stn[j].decided = 1;
		adj = m.cities[pre];
	}

	mincost = stn[dest].cost; 
	pre = dest; int i = 0;
	while (pre != src) {
		pre = stn[pre].pre_way.here;
		i++;
	}
	pathlen = i;
	i--;  pre = dest;
	for (; i >= 0; i--) {
		path[i]= stn[pre].pre_way;
		pre = path[i].here;
	}
}

void Mincost::computeN(Map &m, int n) {
	mincost = 1000000;
	Mincost mc_nested;
	Fact f;
	int all = f.fact(n);
	int j = 0;
	int passBest[3];
	
	for (int i = 0; i<n; i++) {
		passBest[i] = pass[i];
	}
	
	for (int i = 0; i<all; i++) {
			//compute a cheap route for a particular city-pailie:
			int money = 0;
			mc_nested.src = src;
			for (int cnt = 0; cnt <n; cnt++) {
				mc_nested.dest = pass[cnt];
				mc_nested.compute0(m);
				money += mc_nested.mincost;
				mc_nested.src = mc_nested.dest; // src ,dest are places
			}
			mc_nested.dest = dest;
			mc_nested.compute0(m);
			money += mc_nested.mincost;

			
			if (money < mincost) {
				mincost = money;
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
	mc_nested.src = src;
	for (int cnt = 0; cnt < n; cnt++) {
		mc_nested.dest = passBest[cnt];
		mc_nested.compute0(m);
		{
			for (int i = 0; i < mc_nested.pathlen; i++) {
				path[pathlen + i] = mc_nested.path[i];  // 按照从src到dest的正常顺序的
			}
			pathlen += mc_nested.pathlen;
		}
		mc_nested.src = mc_nested.dest; // src ,dest are places
	}
	mc_nested.dest = dest;
	mc_nested.compute0(m);
	{
		for (int i = 0; i < mc_nested.pathlen; i++) {
			path[pathlen + i] = mc_nested.path[i];  // 按照从src到dest的正常顺序的
		}
		pathlen += mc_nested.pathlen;
	}// 至此完成 完整线路的计算 

}

