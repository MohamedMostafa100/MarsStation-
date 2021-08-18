#pragma once
#include <iostream>
#include <string>
using namespace std;
#include "MarsStation.h"

class UI
{
private:
	int x;
public:
	int Input();
	bool waitPressEnter();
	void IgnoreEnterPress();
	void Output(string s);
};


int UI::Input()
{
	x = 0;
	cout << "Welcome to MarsStation" << endl << "Please enter mod of operation" << endl << "Interactive Mode:1" << endl << "Step-By-Step Mode:2" << endl << "Silent Mode:3" << endl;
	cout << "Mod of operation: ";
	cin >> x;
	if (x == 1)
	{
		cout << "Welcome to Interactive mode"<<endl;
		return x;
	}
	else if (x == 2)
	{
		cout << "Welcome to Step-By-Step mode" << endl;
		return x;
	}
	else if (x == 3)
	{
		cout << "Welcome to Silent mode" << endl;
		cout << "Please check output file" << endl;
		return x;
	}
	else
	{
		return 0;	
	}
}

inline bool UI::waitPressEnter()
{
	while (cin.get()!= '\n')
	{ 
		continue;
	}
	return true;
}

inline void UI::IgnoreEnterPress()
{
	cin.ignore();
}

void UI::Output(string s)
{
	cout << s;
}
