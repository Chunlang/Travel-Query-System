#pragma once
#include <iostream>
#include <fstream>
const int numCities = 12;
using namespace std;

class Clock {
public:
	int day, hour;
	void add(int go,int get) {
		day += get / 24;
		if (hour > go) 
			day++;
		hour = get % 24;
	}
	bool change;
	Clock minclk(Clock &a, Clock &b) {
		Clock ret = a;
		change = false;
		if (a.day > b.day || (a.day == b.day&&a.hour > b.hour)) { 
			ret = b; 
			change = true;
		}

		return ret;
	}
	void inc() {
		hour++;
		if (hour == 24) {
			day++;
			hour = 0;
		}
	}
	int minus(Clock &clk) {
		return hour - clk.hour + (day - clk.day) * 24;
	}
};
class Fact {
public:
	int fact(int n) {
		if (n == 0) return 1;
		return n*fact(n - 1);
	}
};
