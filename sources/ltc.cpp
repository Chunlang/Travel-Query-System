#include "stdafx.h"
#include "ltc.h"

void LTC::compute0(Map &m) {//假设已知 t0 src dest（这些事由主函数的initial来做）
	maxcost = 1000000;
	mincost = 1000000;
	pathlen = 0;
	routePara rP;
	Mintime mt;  Mincost mc;
	mt.src = src; mt.dest = dest;
	mc.src = src; mc.dest = dest;
	mt.init();
	mt.compute0(m);
	if (mt.mintime > t_limit) {
		cout << t_limit << " hours is too short, we need at least " << mt.mintime << " hours.\n";
		return;
	}
	maxcost = rP.computeCost(mt.pathlen, mt.path);
	sta stn[numCities];
	stn[src].candidate = false;
	stn[dest].t_d = 0; stn[dest].c_d = 0;
	for (int i = 0; i < numCities; i++) {
		if (i != src&&i != dest) {
			mt.pass[0] = i; mc.pass[0] = i;
			mt.computeN(m, 1);
			mc.computeN(m, 1);
			stn[i].t_inf = mt.mintime;
			if (stn[i].t_inf > t_limit) stn[i].candidate = false;
			stn[i].c_inf = mc.mincost;
			if (stn[i].c_inf > maxcost) stn[i].candidate = false;
		}
	}
	for (int i = 0; i < numCities; i++) {
		if (stn[i].candidate&&i!=dest) {
			mt.src = i; mc.src = i;
			mt.compute0(m); stn[i].t_d = mt.mintime;
			mc.compute0(m); stn[i].c_d = mc.mincost;
		}
	}
	// generate the routeTree:
	routeNode* root = new routeNode;
	root->arrive = t0;
	root->init();
	root->city = src;
	
	for (int i = 0; i < numCities; i++)
		root->candidate[i] = stn[i].candidate;
	// add dest 
	City *adj = m.cities[src];
	while (adj) {
		if (adj->city==dest) {
			Way* w = adj->ways;
			while (w) {
				root->arrive.hour = w->go_time;
				root->arrive.day = t0.day;
				if (w->go_time < t0.hour) {
					root->arrive.day = t0.day + 1;
				}
				maxcost=root->addNode(*w,t_limit,maxcost,stn); //只有在已到达dest时，使用 maxcost=...
				w = w->next; 
			}
			
			break;
		}
		adj = adj->next;
	}
	//root->candidate[adj->city]
	for (int i = 0; i < numCities; i++) {
		if (i != src&&i != dest&&stn[i].c_inf > maxcost)
			stn[i].candidate = false;
	}
	for (int i = 0; i < numCities; i++)
		root->candidate[i] = stn[i].candidate;
	adj = m.cities[src];
	while (adj) {
		if (root->candidate[adj->city]&&adj->city!=dest) {
			Way* w = adj->ways;
			while (w) {
				root->arrive.hour = w->go_time;
				root->arrive.day = t0.day;
				if (w->go_time < t0.hour) {
					root->arrive.day = t0.day + 1;
				}
				root->addNode(*w, t_limit, maxcost, stn); //只有在已到达dest时，使用 maxcost=...
				w = w->next;
			}
		}
		adj = adj->next;
	}
	// 以上为初始化
	for (int i = 1; i < numCities; i++) {
		bool stop=root->traverAdd(i, dest, maxcost, m, t_limit, stn);
		if (stop) break;
	}
	mincost = maxcost;
	routeNode* ans=root->traverFind(dest);
	if (ans) {
		routeNode* pre = ans;
		while (pre->city != src) {
			pathlen++;
			pre = pre->pre_node;
		}
		pre = ans;
		for (int i = 0; i < pathlen; i++) {
			path[pathlen - 1 - i] = pre->pre_way;
			pre= pre->pre_node;
		}
	}
}

bool LTC::DFS(int i, Way* pathCpy, Map &m, nextP* nextpass) {
	bool ret = false;
	routePara rP;
	City* adj = m.cities[nextpass[i - 1].city];
	while (adj) {
		if (adj->city == nextpass[i].city) {
			Way *w = adj->ways;
			while (w) {
				path[i] = *w;
				if (pathlen == i + 1) {
					if (t_limit >= rP.computeTime(pathlen, path)) {
						int currentCost = rP.computeCost(pathlen, path);
						if (mincost>currentCost) {
							mincost = currentCost;
							for (int j = 0; j<pathlen; j++) {
								pathCpy[j] = path[j];
							}
							ret=true; 
						}	
					}
				}
				else {
					ret = DFS(i + 1, pathCpy, m, nextpass)||ret;
				}
				w = w->next;
			}
			break;
		}
		adj = adj->next;
	}
	return ret;
}

void LTC::computeN(Map &m, int n) {
	maxcost = 1000000;
	mincost = 1000000;
	pathlen = 0;
	//  given  t0, numPass,  pass[3] (content is constant)  maxcost   t_limit=min(t_limkit, mincost_t) 
	//  left as-small-as-possible t/cost for A,B,C (passCities   pass[i])    
	
	Mincost mc;
	routePara rP;
	mc.src = src; mc.dest = dest;
	for (int i = 0; i < 3; i++) {
		mc.pass[i] = pass[i];
	}
	mc.computeN(m, n);
	if (rP.computeTime(mc.pathlen, mc.path) <= t_limit) {
		this->mincost = mc.mincost;
		this->pathlen = mc.pathlen;
		for (int i = 0; i < pathlen; i++)
			path[i] = mc.path[i];
		return;
	};
	Mintime mt;
	mt.src = src; mt.dest = dest;
	mt.init();
	for (int i = 0; i < 3; i++) {
		mt.pass[i] = pass[i];
	}
	mt.computeN(m, n);
	if (mt.mintime > t_limit) {
		cout << t_limit << " hours is too short, we need at least " << mt.mintime << " hours.\n";
		return;
	}
	maxcost = rP.computeCost(mt.pathlen, mt.path);
	mincost = maxcost;
	// 路径初始化：以最小时间路径为模板
	nextP nextpass[4];
	int numPass = n; int cnt[3] = { 0 }; int count = 0;
	for (int i = 0; i < mt.pathlen; i++) {
		for (int j = 0; j < numPass; j++) {
			if (mt.path[i].there == pass[j]&&cnt[j]==0) {
				nextpass[count].city = pass[j];
				count++;
				cnt[j]++;
				break;
			}
		}
		if (count == numPass) break;
	}
	 // & initialize it
	nextpass[n].city = dest; nextpass[n].c_d = 0; nextpass[n].t_d = 0;
	/*
	mc.src = src; mc.dest = nextpass[0].city;
	mc.compute0(m); pathlen = mc.pathlen;
	for (int i = 0; i < pathlen; i++)
		path[i] = mc.path[i];
	for (int i = 0; i < numPass; i++) {
		mc.src = nextpass[i].city; mc.dest = nextpass[i + 1].city;
		mc.compute0(m); 
		for (int j = 0; j < mc.pathlen; j++) {
			path[pathlen + j] = mc.path[j];
		}
		pathlen += mc.pathlen;
	}
	
	
	int realtime=rP.computeTime(pathlen, path);

	if (realtime<t_limit) {
		return;
	}
	else {
		mt.src = src; mt.dest = dest;
		mt.init();
		for (int i = 0; i < 3; i++) {
			mt.pass[i] = pass[i];
		}
		mt.computeN(m, n);
		mincost = rP.computeCost(mt.pathlen, mt.path);
		this->pathlen = mt.pathlen;
		for (int i = 0; i < pathlen; i++)
			path[i] = mt.path[i];
		return;
	}
	*/


	// 下面为补充部分：
	City* adj;
	adj=m.cities[src];
	pathlen=numPass+1;
	Way pathCpy[4];
	bool flag=false;
	while(adj){
		if(adj->city==nextpass[0].city){
			Way *w=adj->ways;
			while(w){
				path[0]=*w;
				
				{
					flag = DFS(1, pathCpy, m, nextpass)||flag; //注意DFS要放在||左侧
				}
				w=w->next;
			}
			break;
		}
		adj=adj->next;
	}
	if (flag) { 
		for (int i = 0; i < pathlen; i++)
			path[i] = pathCpy[i];
		return; 
	}
	else {
		mt.src = src; mt.dest = dest;
		mt.init();
		for (int i = 0; i < 3; i++) {
			mt.pass[i] = pass[i];
		}
		mt.computeN(m, n);
		mincost = rP.computeCost(mt.pathlen, mt.path);
		this->pathlen = mt.pathlen;
		for (int i = 0; i < pathlen; i++)
			path[i] = mt.path[i];
		return;
	}

	
	
	
	
}
