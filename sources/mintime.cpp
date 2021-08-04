#include "stdafx.h"
#include "mintime.h"

void Mintime::compute0(Map &m) {
	//t0--t1  
	Clock t = t0, t_best; // t_best ����ѡ�е��Ǹ�
	Earliest earliest; earliest.src = src; earliest.dest = dest;
	mintime = 100000;
	while (t.day != t1.day || t.hour != t1.hour) {
		earliest.start = t;
		earliest.compute(m);  // tΪ�������ʱ�������� ����
		// �����������ͬ��ʼʱ�̼����������·����ȫһ���ģ���arriveʱ���һ������duration�ز�ͬ
		int duration = earliest.arrive.hour - earliest.start.hour + (earliest.arrive.day - earliest.start.day) * 24;
		if (duration < mintime) {
			mintime = duration;
			t_best = t;
		}
		t.inc();
	}
	earliest.start = t_best;
	earliest.compute(m); // ÿ����·���洢̫�Ŀռ䣬���Խ�����t_best��Ϣ����compute�ָ�������·��Ϣ
	for (int i = earliest.pathlen - 1; i >= 0; i--) {
		path[earliest.pathlen - 1 - i] = earliest.path[i];  // Mintime.path �ǰ��մ�src��dest������˳���
	}
	pathlen = earliest.pathlen;
}


// ����ѭ������  ͳһ���� 3�� pass-by������ ����  
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
		if (tMin < mintime) { // ���浱ǰ����
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
	
	// �ü���ָ� ��·��Ϣ��
	pathlen = 0;
	Earliest earliest;
	earliest.start = tBest;
	earliest.src = src;
	for (int cnt = 0; cnt < n; cnt++) {
		earliest.dest = passBest[cnt];
		earliest.compute(m);
		{
			for (int i = 0; i < earliest.pathlen ; i++) {
				path[pathlen+i] = earliest.path[earliest.pathlen-1-i];  // Mintime.path �ǰ��մ�src��dest������˳���
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
			path[pathlen + i] = earliest.path[earliest.pathlen - 1 - i];  // Mintime.path �ǰ��մ�src��dest������˳���
		}
		pathlen += earliest.pathlen;
	}// ������� ������·�ļ��� 

}
