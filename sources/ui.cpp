#include "stdafx.h"
#include "ui.h"

req User::uWant(Clock t0) {
	req ret;
	cout<<"���б�����£�\n"
		<< "0 ����,1 �Ϻ�,2 ����,3 ����,4 �Ͼ�,5 ����\n"
		<< "6 ����,7 �ɶ�,8 �人,9 ����,10 ����,11 ��ɳ\n";
	cout << "������������б�ţ�";
	cin >> ret.src;
	cout << "�������յ���б�ţ�";
	cin >> ret.dest;
	cout << "��ѡ�����: 1 ��С���� 2 ��Сʱ�� 3 ��ʱ��С����\n";
	cin >> ret.strategy;
	if (ret.strategy == 3) {
		cout << "���趨��������ʱ (��λ��Сʱ):\n";
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
	
	cout << "�����м���и�����\n";
	cin >> ret.n;
	if (ret.n > 0) {
		cout << "Choose from the following:\n"
			<< "0 ����,1 �Ϻ�,2 ����,3 ����,4 �Ͼ�,5 ����\n"
			<< "6 ����,7 �ɶ�,8 �人,9 ����,10 ����,11 ��ɳ\n";
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
			cout << "��Σ�" << mt.path[i].code << " �������У�" << cityName[mt.path[i].here]
				<< " ������У�" << cityName[mt.path[i].there] << " ����ʱ�䣺" << mt.path[i].go_time
				<< " ����ʱ�䣺" << mt.path[i].get_time << " Ʊ�ۣ�" << mt.path[i].cost << endl;
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
	cout<< "Please choose from: 1ע��  2��¼  3·�߹滮  4��ѯ״̬  5�˳�  6�ر�ϵͳ\n";
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
	// Ϊһ�������û��ṩ·�߹滮����
	int id[100];
	cout << "��������Ҫ�滮·�ߵ��û�����";
	int num; cin >> num;
	
	for (int i = 0; i < num; i++) {
		cout << "\n�������û���: ";
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
			cout << "��������\n\n";
			return;
		}
	}
	Clock late; late.day = -1;
	for (int i = 0; i < num; i++) {
		cout << "Ϊ" << u[id[i]].username << "�Ƽ����з�����\n";
		for (int j = 0; j < u[id[i]].pathlen; j++)
			cout << u[id[i]].path[j].code << ": "<<cityName[u[id[i]].path[j].here]<<"-->"
			<< cityName[u[id[i]].path[j].there] <<" Ʊ�ۣ�"<<u[id[i]].path[j].cost<<"\n";
		cout << "Ԥ�ƺ�ʱ��" << u[id[i]].time_predict << "Сʱ  Ԥ�ƻ��ѣ�" << u[id[i]].cost_predict << "Ԫ";
		cout << endl;
		if (u[id[i]].arrive.minus(late) > 0)
			late = u[id[i]].arrive;
	}
	//�滮��·�ߺ󣬿�ʼģ�����й��̣�
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
					cout << "��" << cityName[u[id[i]].location] << "����  ";
				}
				else {
					u[id[i]].location = u[id[i]].path[point[i] / 2 - 1].there;
					event.record(now, u[id[i]], 3);
					cout << "����" << cityName[u[id[i]].location] << "  ";
				}
				thing = true;
			}
		}
		Sleep(500);
		now.inc();
		if (thing)
			cout << endl;
	}
	//����ʱ���Ϊ��������ĵ���ʱ��
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
					cout << "��" << cityName[u[id[i]].location] << "����  ";
				}
				else {
					u[id[i]].location = u[id[i]].path[point[i] / 2 - 1].there;
					event.record(now, u[id[i]], 3);
					cout << "����" << cityName[u[id[i]].location] << "  ";
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
			cout << "��ǰʱ�䣺day " << now.day << " hour " << now.hour
			 << " �����ڵ�λ�ã�" << cityName[u[i].location] << endl;
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
	cout << "ϵͳ�ѳɹ��ر�\n";
}
Clock UI::follow(Clock previous, User user, int point) {// ������һ��ʱ��ڵ��ֵ point�����Ѿ���ȥ��ʱ��ڵ���
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