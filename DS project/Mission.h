#pragma once


#include "Rover.h"
#include<cmath>



class Mission
{
private:
	int ID;
	int FD;
	int WD;
	int ED;
	int CD;
	char Type;
	int TLOC;
	int MDUR;
	int SIG;
	int PRI;
	Rover* rover;
public:
	Mission(char typ,int e, int i, int tl, int md, int sig);
	void AssignRover(Rover* rvr);
	void setWD(int currentDay);
	int GetExecutionDays();
	int GetCompletionDay();
	int GetID();
	int GetFD();
	int GetWD();
	int GetED();
	char getType();
	void setPriority(int p);
	int getPriority();
	Rover* getRover();
};

inline Mission::Mission(char typ,int e, int i, int tl, int md, int sig)
{
	Type = typ;
	WD = 0;
	FD = e;
	ID = i;
	TLOC = tl;
	MDUR = md;
	SIG = sig;
}

inline void Mission::AssignRover(Rover* rvr)
{
	rover = rvr;
}

inline void Mission::setWD(int currentDay)
{
	WD = currentDay - FD;
}

inline int Mission::GetExecutionDays()
{
	int executiondays = ceil((float)((float)((float)(TLOC / rover->getspeed()) / 25)*2) + MDUR);
	return executiondays; 
}

inline int Mission::GetCompletionDay()
{
	ED = GetExecutionDays();
	CD = ED + FD + WD;
	return CD;
}

inline int Mission::GetID()
{
	return ID;
}

inline int Mission::GetFD()
{
	return FD;
}

inline int Mission::GetWD()
{
	return WD;
}

inline int Mission::GetED()
{
	return ED;
}

inline char Mission::getType()
{
	return Type;
}

inline void Mission::setPriority(int p)
{
	PRI = p;
}

inline int Mission::getPriority()
{
	return PRI;
}

inline Rover* Mission::getRover()
{
	return rover;
}
