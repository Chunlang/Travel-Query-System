#include "stdafx.h"
#include "map.h"


void  Map::addCity(Table &t) {
	// write table content into a city-city of map;   use when  first time appear this city-city 
	//struct City* c = (struct City*)malloc(sizeof(struct City));
	City *c=new City;
	c->city = t.there;

	c->ways = new Way;
	strcpy_s(c->ways->code, t.code);
	c->ways->here = t.here;
	c->ways->there = t.there;
	c->ways->tool = t.tool;
	c->ways->cost = t.cost;
	c->ways->go_time = t.go_time;
	c->ways->get_time = t.get_time;
	c->ways->next = NULL;

	c->next = cities[t.here];
	cities[t.here] = c;
}

void City::addWay(Table &t) {
	Way *way = new Way;
	strcpy_s(way->code, t.code);
	way->here = t.here;
	way->there = t.there;
	way->tool = t.tool;
	way->cost = t.cost;
	way->go_time = t.go_time;
	way->get_time = t.get_time;
	way->next = ways;
	ways = way;
}

void Map::input() {
	//	file_in>>numC>>cities>>transport_tables;

	ifstream myfile("finalData.txt");
	Table table[9000];
	for (int i = 0; i<8136; i++) {
		myfile >> table[i].code >> table[i].here >> table[i].there >> table[i].go_time >> table[i].get_time >> table[i].cost;
	}
	/*for (int i = 0; i<180; i++) {
		table[i].here--;
		table[i].there--;
	}*/


	for (int i = 0; i<8136; i++) {
		{
			if (i == 0 || table[i].here != table[i - 1].here) addCity(table[i]);
			else if (table[i].there != table[i - 1].there) addCity(table[i]);
			else cities[table[i].here]->addWay(table[i]);
		}
	}
	myfile.close();
}

Clock City::early(Clock &clk) {
	e.day = -1;
	Way* w = ways;
	Clock c;
	while (w) {
		c = clk;
		c.add(w->go_time, w->get_time);
		if (e.day < 0 || (e.day == c.day&&e.hour > c.hour) || e.day>c.day) {
			e = c;
			w_early = *w;
		}

		w = w->next;
	}
	return e;
}
int City::cheap() {
	int min = 100000;
	Way* w = ways;
	if (w) {
		min = w->cost; 
		w_cheap = *w;
	}
	while (w) {
		if (w->cost < min) { 
			min = w->cost; 
			w_cheap = *w;
		}
		w = w->next;
	}
	return min;
}
