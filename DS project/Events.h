#pragma once
#include"Mission.h"

class Events
{
public:
	virtual Mission* Execute() = 0;
};

