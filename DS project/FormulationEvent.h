#pragma once
#include "Events.h"
#include "Mission.h"


class FormulationEvent: public Events
{
private:
    char TYP;
    int ED;
    int ID;
    int TLOC;
    int MDUR;
    int SIG;
public:
    FormulationEvent(char t,int e,int i,int tl,int md,int sig);
    int getED();
    int getpriority();
    char gettype();
    Mission* Execute();

    ~FormulationEvent();
};


FormulationEvent::FormulationEvent(char t, int e, int i, int tl, int md, int sig)
{
    TYP = t;
    ED = e;
    ID = i;
    TLOC = tl;
    MDUR = md;
    SIG = sig;
}

inline int FormulationEvent::getED()
{
    return ED;
}

inline int FormulationEvent::getpriority()
{
    int pri = ED + SIG + MDUR + TLOC; //priority equation of waiting emergency
    return pri;
}

inline char FormulationEvent::gettype()
{
    return TYP;
}


Mission* FormulationEvent::Execute()
{
    Mission* newmission = new Mission(TYP,ED, ID, TLOC, MDUR, SIG);
    return newmission;
}


FormulationEvent::~FormulationEvent()
{
}
