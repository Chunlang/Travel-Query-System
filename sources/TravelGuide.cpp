//
//  TravelGuide.cpp
//
//  Created by Chunlang Hu. on May 20, 2019
//  Copyright  All rights reserved.
//

#include "stdafx.h"
#include "ui.h"
int main()
{
	UI ui;
	ui.init();
	cout << "Welcome to the Travel Guide System!\n\n";
	while (1) {
		int choice = ui.welcome();		
		if(choice==6) break;
	}
	
	ui.update();
				// actually it works quite well   
    return 0;
}

