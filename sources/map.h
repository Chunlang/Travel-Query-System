#pragma once
#include "stdafx.h"
#include "common.h"
class Table {
public:
	int here, there;
	int go_time, get_time;
	int cost;
	char tool;
	char code[30];
};

class Way{
public:
	int here, there; // here 出发地  there 到达地
	int go_time,get_time;
	int cost;
	char tool; // P or T or B
	char code[30];  // z107 G905
	Way *next;
};

class City{  // dest cities
public:
	int city;// from 0 to 11
	Way *ways;   // ways from src to dest
	City* next;
	void addWay(Table &t);
	Way w_cheap;
	Clock e; Way w_early;
	Clock early(Clock &clk); // this func means to determine the earliest way to arrive this city from src 
	int cheap();
};

class Map{
public:
	City* cities[numCities];  // array of source cities

	void init(){
		for (int i = 0; i<numCities; i++) {
			cities[i] = NULL;
		}
	}
	void addCity(Table &t);
	void input();
};

class routePara {
public:
	// deduce route parameters, such as totaltime/totalcost, from path info
	int time;
	int cost;
	Clock start, arrive;
	int computeTime(int len, Way* path) {
		start.day = 0; start.hour = path[0].go_time;
		arrive = start;
		for (int i = 0; i < len; i++) {
			arrive.add(path[i].go_time, path[i].get_time);
		}
		time = arrive.hour - start.hour + (arrive.day - start.day) * 24;
		return time;
	}
	int computeCost(int len, Way* path) {
		cost = 0;
		for (int i = 0; i < len; i++) {
			cost += path[i].cost;
		}
		return cost;
	}
};