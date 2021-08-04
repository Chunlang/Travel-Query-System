#pragma once
#include "ltc.h"
#include <Windows.h>
const char cityName[12][30] = { "北京","上海","深圳","杭州","南京","广州",
"重庆","成都","武汉","宁波","苏州","长沙" };
typedef struct Requirements {
	int src, dest;
	int strategy;
	int t_limit;
	Clock t0, t1;
	int n;
	int pass[3];
}req;

class User {
public:
	int id;
	bool online;
	char username[30];
	int location;
	int pathlen;
	Way path[44];
	int time_predict;
	int cost_predict;
	Clock arrive;
	req reqs;
	req uWant(Clock);
	void travel(Map &);
	User() {
		online = false;
		pathlen = 0;
	}

};

class Event {
public:
	void record(Clock t, User u, int event) {
		if (event > 0 && event < 5) {
			ofstream logfile("travel.log", ios::app);
			logfile << "day " << t.day << " hour " << t.hour
				<< " 用户" << u.username;

			switch (event) {
			case 1: {
				logfile << "注册成功\n";
				break;
			}
			case 2: {
				logfile << "登录系统" << endl;
				break;
			}
			case 3: {
				logfile << "所处城市变为：" << cityName[u.location] << endl;
				break;
			}
			case 4: {
				logfile << "退出系统" << endl;
				break;
			}
			}

			logfile.close();
		}
	}

};

class UI {//user interface
public:
	Map m;
	Clock now;
	int numUser;
	int num_online;
	User u[100];
	UI() {
		numUser = 0;
		num_online = 0;
	}
	void init();
	int welcome();
	void signUp();
	int logIn();
	void compute();
	void query();
	void logOut();
	void exit();
	void update();
	Clock follow(Clock,User,int);
};

