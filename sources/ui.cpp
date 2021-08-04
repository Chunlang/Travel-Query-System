#include "stdafx.h"
#include "ui.h"

req User::uWant(Clock t0) {
	req ret;
	cout<<"城市编号如下：\n"
		<< "0 北京,1 上海,2 深圳,3 杭州,4 南京,5 广州\n"
		<< "6 重庆,7 成都,8 武汉,9 宁波,10 苏州,11 长沙\n";
	cout << "请输入出发城市编号：";
	cin >> ret.src;
	cout << "请输入终点城市编号：";
	cin >> ret.dest;
	cout << "请选择策略: 1 最小费用 2 最小时间 3 限时最小费用\n";
	cin >> ret.strategy;
	if (ret.strategy == 3) {
		cout << "请设定最大允许耗时 (单位：小时):\n";
		cin >> ret.t_limit;
	}
	
	{
		ret.t0 = t0;
		ret.t1 = t0; 
		if (t0.hour == 0)
			ret.t1.hour = 23;
		else {
			ret.t1.day++;
			ret.t1.hour--;
		}
	}
	
	cout << "经过中间城市个数：\n";
	cin >> ret.n;
	if (ret.n > 0) {
		cout << "Choose from the following:\n"
			<< "0 北京,1 上海,2 深圳,3 杭州,4 南京,5 广州\n"
			<< "6 重庆,7 成都,8 武汉,9 宁波,10 苏州,11 长沙\n";
		for (int i = 0; i < ret.n; i++)
			cin >> ret.pass[i];
	}

	return ret;
}

void User::travel(Map &map) {
	switch (reqs.strategy) {
	case 2: {
		// quickest
		Mintime mt;
		mt.src = reqs.src;
		mt.dest = reqs.dest;
		mt.t0 = reqs.t0;
		mt.t1 = reqs.t1;
		for (int i = 0; i < reqs.n; i++) {
			mt.pass[i] = reqs.pass[i];
		}
		if (reqs.n <= 0) mt.compute0(map);
		else if (reqs.n <= 3) mt.computeN(map, reqs.n);
		else mt.computeN(map, 3);
		// here should be an update fot t0 and userdata:
		/*cout << "min time is :" << mt.mintime << endl;
		cout << "One feasible path is :\n";
		for (int i = 0; i < mt.pathlen; i++) {
			cout << "班次：" << mt.path[i].code << " 出发城市：" << cityName[mt.path[i].here]
				<< " 到达城市：" << cityName[mt.path[i].there] << " 出发时间：" << mt.path[i].go_time
				<< " 到达时间：" << mt.path[i].get_time << " 票价：" << mt.path[i].cost << endl;
		}*/
		pathlen = mt.pathlen;
		for (int i = 0; i < pathlen; i++)
			path[i] = mt.path[i];
		break;
	}
	case 3: {
		// cheapest with time limit
		LTC ltmc;
		ltmc.src = reqs.src;
		ltmc.dest = reqs.dest;
		ltmc.t0 = reqs.t0;
		ltmc.t_limit = reqs.t_limit;
		for (int i = 0; i < reqs.n; i++) {
			ltmc.pass[i] = reqs.pass[i];
		}
		if (reqs.n <= 0) ltmc.compute0(map);
		else if (reqs.n <= 3) ltmc.computeN(map, reqs.n);
		else ltmc.computeN(map, 3);
		//
		pathlen = ltmc.pathlen;
		for (int i = 0; i < pathlen; i++)
			path[i] = ltmc.path[i];
		break;
	}
	default: {
		// cheapest
		Mincost mc;
		mc.src = reqs.src;
		mc.dest = reqs.dest;
		for (int i = 0; i < reqs.n; i++) {
			mc.pass[i] = reqs.pass[i];
		}
		if (reqs.n <= 0) mc.compute0(map);
		else if (reqs.n <= 3) mc.computeN(map, reqs.n);
		else mc.computeN(map, 3);
		//
		pathlen = mc.pathlen;
		for (int i = 0; i < pathlen; i++)
			path[i] = mc.path[i];
		break;
	}
	}
	routePara rP;
	time_predict = rP.computeTime(pathlen, path);
	cost_predict = rP.computeCost(pathlen, path);
	arrive = reqs.t0;
	int go, get;
	go = path[0].go_time;  get = go + time_predict;
	arrive.add(go, get);
}

void UI::init() {
	m.init();
	m.input();
	ifstream file("UserData.txt");
	file >> numUser;
	file >> now.day >> now.hour;
	for (int i = 0; i < numUser; i++) {
		u[i].id = i;
		file >> u[i].username;
		file >> u[i].location;
	}
	file.close();
}
void UI::update() {
	ofstream file("UserData.txt");
	file << numUser << endl;
	file << now.day << '\t' << now.hour << endl;
	for (int i = 0; i < numUser; i++) {
		file << u[i].username<<" ";
		file << u[i].location<<endl;
	}
	file.close();
}
int UI::welcome() {
	cout<< "Please choose from: 1注册  2登录  3路线规划  4查询状态  5退出  6关闭系统\n";
	int choice;
	cin >> choice;
	switch (choice) {
	case 1:
		signUp();
		break;
	case 2:
		logIn();
		break;
	case 3:
		compute();
		break;
	case 4:
		query();
		break;
	case 5:
		logOut();
		break;
	default:
		exit();
		break;
	}
	
	return choice;
}
void UI::signUp() {
	Event event;
	while (1) {
		cout << "Please input your username:\n";
		char name[30];
		cin >> name;
		int i = 0; 
		// debug: is numUser right?
		for (; i < numUser; i++) {
			if (!strcmp(u[i].username, name)) break;
		}
		if (i == numUser) {
			u[i].id = i;
			strcpy_s(u[i].username, name);
			u[i].location = -1;
			numUser++;
			cout << "You have signed up successfully!\n";
			event.record(now, u[i], 1);
			break;
		}
		else {
			cout << "This name has been used, please try another name.\n";
		}
	}
}
int UI::logIn() {
	Event event;
	while (1) {
		cout << "Please input your username:\n";
		char name[30];
		cin >> name;
		int i = 0;
		for (; i < numUser; i++) {
			if (!strcmp(u[i].username, name)) break;
		}
		if (i<numUser) {
			u[i].online = true;
			cout << "You've logged in!\n";
			event.record(now, u[i], 2);
			num_online++;
			return i;
		}
		else {
			cout << "Username wrong. 0 try again  1 exit\n";
			int choice;
			cin >> choice;
			if (choice) return -1;
		}
	}
}
void UI::compute() {
	// 为一个或多个用户提供路线规划服务
	int id[100];
	cout << "请输入需要规划路线的用户数：";
	int num; cin >> num;
	
	for (int i = 0; i < num; i++) {
		cout << "\n请输入用户名: ";
		char name[30]; cin >> name;
		int j = 0;
		for (; j < numUser; j++) {
			if (strcmp(name, u[j].username) == 0 && u[j].online) {
				u[j].reqs=u[j].uWant(now);
				id[i] = j;
				u[j].travel(m);
				break;
			}
		}
		if (j == numUser) {
			cout << "输入有误\n\n";
			return;
		}
	}
	Clock late; late.day = -1;
	for (int i = 0; i < num; i++) {
		cout << "为" << u[id[i]].username << "推荐旅行方案：\n";
		for (int j = 0; j < u[id[i]].pathlen; j++)
			cout << u[id[i]].path[j].code << ": "<<cityName[u[id[i]].path[j].here]<<"-->"
			<< cityName[u[id[i]].path[j].there] <<" 票价："<<u[id[i]].path[j].cost<<"\n";
		cout << "预计耗时：" << u[id[i]].time_predict << "小时  预计花费：" << u[id[i]].cost_predict << "元";
		cout << endl;
		if (u[id[i]].arrive.minus(late) > 0)
			late = u[id[i]].arrive;
	}
	//规划好路线后，开始模拟旅行过程：
	Event event;
	Clock previous=now;
	int time_total= late.minus(previous);
	int point[100];
	for (int i = 0; i < 100; i++) {
		point[i] = 0;
	}
	for (int t = 0; t < time_total; t++) {
		bool thing = false;
		for (int i = 0; i < num; i++) {
		  for(int repeat=0;repeat<2;repeat++)
			if (now.minus(follow(previous, u[id[i]], point[i]))==0) {
				if (!thing)
					cout << "day " << now.day << " hour " << now.hour << "\t";
				point[i]++;
				cout << u[id[i]].username;
				if (point[i] % 2) {
					if (u[id[i]].location != u[id[i]].path[point[i] / 2].here) {
						u[id[i]].location = u[id[i]].path[point[i] / 2].here;
						event.record(now, u[id[i]], 3);
					}
					cout << "从" << cityName[u[id[i]].location] << "出发  ";
				}
				else {
					u[id[i]].location = u[id[i]].path[point[i] / 2 - 1].there;
					event.record(now, u[id[i]], 3);
					cout << "到达" << cityName[u[id[i]].location] << "  ";
				}
				thing = true;
			}
		}
		Sleep(500);
		now.inc();
		if (thing)
			cout << endl;
	}
	//以下时间点为最晚到达组的到达时间
	bool thing = false;
	for (int i = 0; i < num; i++) {
		for (int repeat = 0; repeat<2; repeat++)
			if (now.minus(follow(previous, u[id[i]], point[i])) == 0) {
				if (!thing)
					cout << "day " << now.day << " hour " << now.hour << "\t";
				point[i]++;
				cout << u[id[i]].username;
				if (point[i] % 2) {
					if (u[id[i]].location != u[id[i]].path[point[i] / 2].here) {
						u[id[i]].location = u[id[i]].path[point[i] / 2].here;
						event.record(now, u[id[i]], 3);
					}
					cout << "从" << cityName[u[id[i]].location] << "出发  ";
				}
				else {
					u[id[i]].location = u[id[i]].path[point[i] / 2 - 1].there;
					event.record(now, u[id[i]], 3);
					cout << "到达" << cityName[u[id[i]].location] << "  ";
				}
				thing = true;
			}
	}
	if (thing)
		cout << endl;


}
void UI::logOut() {
	Event event;
	while (1) {
		cout << "Please input your username:\n";
		char name[30];
		cin >> name;
		int i = 0;
		for (; i < numUser; i++) {
			if (!strcmp(u[i].username, name)) break;
		}
		if (i<numUser&&u[i].online) {
			u[i].online = false;
			cout << "You've logged out!\n";
			event.record(now, u[i], 4);
			num_online--;
			return;
		}
		else {
			cout << "Username wrong. 0 try again  1 exit\n";
			int choice;
			cin >> choice;
			if (choice) return;
		}
	}
}
void UI::query() {
	while (1) {
		cout << "Please input your username:\n";
		char name[30];
		cin >> name;
		int i = 0;
		for (; i < numUser; i++) {
			if (!strcmp(u[i].username, name)) break;
		}
		if (i<numUser&&u[i].online) {
			cout << "当前时间：day " << now.day << " hour " << now.hour
			 << " 您现在的位置：" << cityName[u[i].location] << endl;
			return;
		}
		else {
			cout << "Username wrong. 0 try again  1 exit\n";
			int choice;
			cin >> choice;
			if (choice) return;
		}
	}
}
void UI::exit() {
	Event event;
	for (int i = 0; i < numUser; i++) {
		if (u[i].online)
			event.record(now,u[i], 4);
	}
	cout << "系统已成功关闭\n";
}
Clock UI::follow(Clock previous, User user, int point) {// 计算下一个时间节点的值 point代表已经过去的时间节点数
	Clock ret;
	if (point >= 2 * user.pathlen) {
		ret.day = -1; ret.hour = -1;
		return ret;
	}
	ret = previous;
	for (int k = 0; k <= point; k++) {
		int go, get;
		if (k % 2) {
			go = user.path[k / 2].go_time;
			get = user.path[k / 2].get_time;
		}
		else {
			go = user.path[k / 2].go_time;
			get = go;
		}
		ret.add(go, get);
	}
	return ret;
}