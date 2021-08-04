#include "stdafx.h"
#include "earliest.h"

void Earliest::compute(Map &m){
	// this is for getting the earliest time of from A city to B city  
	// there are intermediate cities     
	//  so we exploit the generic Dijkstra algo
	//  first yo got a map and you are gonna deal with it 
	// class earliest include attributes like start,arrive src dest
	// with start you get the accurate beginning time    
	// to store the states of  each city we need a container including decided, pre-way, and earliest arrive time 
	cc c[numCities]; // from src to next    m.cities[src]!=NULL then it has connected cities 
	if (m.cities[src]) { // this pointer is not null  
		c[src].decided = true;
		c[src].arrive = start;  // initialize the src node  
		// when a node gets decided , we shall update all its adjs then choose what is the earliest to arrive to delete 
		// from all undecided nodes   
		int pre = src;  // pre is a newly decided city 
		City* adj = m.cities[pre];
		while (pre != dest) {
			while (adj) {
				//gonna update the arrive time of the adj city:
				if (!c[adj->city].decided) {
					int now = adj->city;
					if (c[now].pre_city < 0) {
						c[now].arrive=adj->early(c[pre].arrive);  // from pre to adj  earliest direct edge  
						// w_early,e of adj updated;  
						c[now].pre_city = pre;
						c[now].pre_way = adj->w_early;
					}
					else { // adj已经有pre_way  要比较 原arrive和从pre出发的 优劣 
						adj->early(c[pre].arrive);
						if ((c[now].arrive.day == adj->e.day&&c[now].arrive.hour > adj->e.hour) || c[now].arrive.day > adj->e.day)
						{
							c[now].arrive = adj->e;
							c[now].pre_city = pre;
							c[now].pre_way = adj->w_early;
						}
					}
				}
				adj = adj->next;  // all cities on this chain are directly connected with pre as a dest; the src can be on this chain
			}

			// find the min arrive time from all undecided cities 
			Clock min;
			min.day = 10000; min.hour = 0;
			for (int i = 0; i < numCities; i++) {
				if (!c[i].decided) {
					min = min.minclk(min, c[i].arrive);
					if (min.change) pre = i;
				}
			}
			c[pre].decided = true;
			adj = m.cities[pre];
			//
		}
		//now the earliest route to dest is found 
		arrive = c[dest].arrive;  
		pre = dest; int i = 0;
		while (pre!=src) {
			path[i] = c[pre].pre_way;
			pre = c[pre].pre_city;  // In fact, c[pre].pre_city=path[i].here;
			i++;
		}
		pathlen = i;
	}
	else {
		cout << "So sad. There is just no way between the two cities.\n";
	}
}
