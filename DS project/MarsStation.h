#pragma once
#include"LinkedQueue.h"
#include"PriorityQueue.h"
#include"Events.h"
#include"FormulationEvent.h"
#include"Mission.h"
#include"Rover.h"
#include"UI.h"
#include<fstream>
#include<string>
#include <chrono>
#include <thread>
#include<time.h>
using namespace std::chrono;
using namespace std::this_thread;



class MarsStation
{
private:
	LinkedQueue<Events*> events;
	LinkedQueue<Mission*> WaitingPolar;
	PriorityQueue<Mission*> WaitingEmergency;
	LinkedQueue<Mission*> CompletedMissions;
	PriorityQueue<Rover*> AvailablePolar;
	PriorityQueue<Rover*> AvailableEmergency;
	PriorityQueue<Mission*> InExecutionMissions;
	PriorityQueue<Rover*> CheckupRovers;
	PriorityQueue<Rover*> MaintainanceRovers;
	LinkedQueue<Mission*> FailedMissions;
	UI* pUI;
	int noEvents;
	int noCompletedMissions;
	int modofoperation;

public:
	MarsStation();
	void ReadFile();
	void SimulateDays();
	void printDailyData(int d);
	void drawline();
	void printMissions(LinkedQueue<Mission*> M);
	void printPriorityMissions(PriorityQueue<Mission*> M);
	void printRovers(PriorityQueue<Rover*> R);
	void printCheckupPolar(PriorityQueue<Rover*> R);
	void printCheckupEmergency(PriorityQueue<Rover*> R);
	void printInExecutionPolar(PriorityQueue<Mission*> temp1);
	void printInExecutionEmergency(PriorityQueue<Mission*> temp1);
	void printCompletedPolar(LinkedQueue<Mission*> temp1);
	void printCompletedEmergency(LinkedQueue<Mission*> temp1);
	void InteractiveMode(int d);
	void Step_by_Step_Mode(int d);
	void PrintOutputFile();
};

inline MarsStation::MarsStation() : pUI(new UI)
{
}

inline void MarsStation::ReadFile()
{
	int noPolar;
	int noEmergency;
	int ID = 1;
	int chPolar;
	int chEmergency;
	int chdur;
	char Type;
	char Typeevent;
	int ED, IDmission, TLOC, MDUR, SIG;
	ifstream infile;
	infile.open("Input file.txt");
	if (infile.fail())
	{
		cout << "ERROR" << endl;
	}
	infile >> noPolar >> noEmergency; 
	int* spPolar = new int[noPolar];
	int* spEmergency = new int[noEmergency];
	//bonus:speed for each rover
	for (int i = 0; i < noPolar; i++)
	{
		infile >> spPolar[i];
	}
	for (int i = 0; i < noEmergency; i++)
	{
		infile >> spEmergency[i];
	}
	infile >> chPolar >> chEmergency >> chdur;
	for (int i = 0; i < noPolar; i++)
	{
		Rover* newrover = new Rover('P',spPolar[i], ID++, chPolar, chdur);
		AvailablePolar.enqueue(newrover,spPolar[i]);
	}
	for (int i = 0; i < noEmergency; i++)
	{
		Rover* newrover = new Rover('E',spEmergency[i], ID++, chEmergency, chdur);
		AvailableEmergency.enqueue(newrover,spEmergency[i]);
	}
	infile >> noEvents;
	int noUnexecutedPolar = 0; //variable to count polar events that will not be excuted if there are no polar rovers
	for (int i = 0; i < noEvents; i++)
	{
		infile >> Typeevent >> Type >> ED >> IDmission >> TLOC >> MDUR >> SIG;
		if (Type != 'P' || !AvailablePolar.isEmpty())//Ignore polar events if there are no polar rovers
		{
			FormulationEvent* newevent = new FormulationEvent(Type, ED, IDmission, TLOC, MDUR, SIG);
			events.enqueue(newevent);
		}
		else
		{
			noUnexecutedPolar++;
		}
	}
	noEvents -= noUnexecutedPolar;
}

inline void MarsStation::SimulateDays()
{
	srand(time(0));
	int x; //bonus: number to randomized
	modofoperation = pUI->Input();
	pUI->IgnoreEnterPress();
	if (modofoperation == 0)
	{
		pUI->Output("Invalid input please restart the program");
		return;
	}
	Events* eve;
	FormulationEvent* feve;
	Mission* newmission;
	noCompletedMissions = 0;
	int Days = 1;
	while (noCompletedMissions != noEvents || CheckupRovers.getcount()!=0)
	{
		x = rand() % 1000; //bonus: Randomizing
		while (!CheckupRovers.isEmpty())
		{
			Rover* rvrcheckup;
			CheckupRovers.peek(rvrcheckup);
			if (Days >= rvrcheckup->get_extraction_Day())
			{
				if (rvrcheckup->gettype() == 'P')
				{
					CheckupRovers.dequeue(rvrcheckup);
					AvailablePolar.enqueue(rvrcheckup,rvrcheckup->getspeed());
				}
				else if (rvrcheckup->gettype() == 'E')
				{
					CheckupRovers.dequeue(rvrcheckup);
					AvailableEmergency.enqueue(rvrcheckup,rvrcheckup->getspeed());
				}
			}
			else
			{
				break;
			}
		}
		while (!MaintainanceRovers.isEmpty())
		{
			Rover* rvrcheckup;
			MaintainanceRovers.peek(rvrcheckup);
			if (Days >= rvrcheckup->get_extraction_main())
			{
				if (rvrcheckup->gettype() == 'P')
				{
					MaintainanceRovers.dequeue(rvrcheckup);
					AvailablePolar.enqueue(rvrcheckup, rvrcheckup->getspeed());
				}
				else if (rvrcheckup->gettype() == 'E')
				{
					MaintainanceRovers.dequeue(rvrcheckup);
					AvailableEmergency.enqueue(rvrcheckup, rvrcheckup->getspeed());
				}
			}
			else
			{
				break;
			}
		}
		while (!InExecutionMissions.isEmpty())
		{
			Mission* mission;
			InExecutionMissions.peek(mission);
			if (x == 5) //bonus: choosing 5 from random values to make missions fail
			{
				Rover* rover=mission->getRover();
				InExecutionMissions.dequeue(mission);
				FailedMissions.enqueue(mission);
				rover->set_extraction_Day(Days + rover->get_checkup_duration());
				rover->set_missions_done(0);
				CheckupRovers.enqueuelowest(rover, rover->get_extraction_Day());
				if (mission->getType() == 'P')
				{
					WaitingPolar.enqueue(mission);
				}
				else if (mission->getType() == 'E')
				{
					WaitingEmergency.enqueue(mission,mission->getPriority());
				}
			}
			if (Days >= mission->GetCompletionDay())
			{
				Rover* rover;
				InExecutionMissions.dequeue(mission);
				CompletedMissions.enqueue(mission);
				rover = mission->getRover();
				rover->Increment_missions();
				if (rover->get_missions_done() == rover->get_missions_checkup())
				{
					rover->set_extraction_Day(Days + rover->get_checkup_duration());
					rover->set_missions_done(0);
					CheckupRovers.enqueuelowest(rover, rover->get_extraction_Day());
				}
				else
				{
					// bonus:if an emergency mission is executed by a polar rover, the rover must be sent to maintenance
					if (rover->gettype() == 'P' && mission->getType() == 'E') 
					{
						rover->set_extraction_main(Days + rover->get_main_Duration());
						MaintainanceRovers.enqueuelowest(rover, rover->get_extraction_main());
					}
					else if (rover->gettype() == 'P')
					{
						AvailablePolar.enqueue(rover,rover->getspeed());
					}
					else if (rover->gettype() == 'E')
					{
						AvailableEmergency.enqueue(rover, rover->getspeed());
					}
				}
				noCompletedMissions++;
			}
			else
			{
				break;
			}
		}
		while (!events.isEmpty())
		{
			events.peek(eve);
			feve = (FormulationEvent*)eve;
			if (Days >= feve->getED())
			{
				newmission = feve->Execute();
				events.dequeue(eve);
				if (feve->gettype() == 'P')
				{
					WaitingPolar.enqueue(newmission);
				}
				else if (feve->gettype() == 'E')
				{
					newmission->setPriority(feve->getpriority());
					WaitingEmergency.enqueue(newmission, newmission->getPriority());
				}
			}
			else
			{
				break;
			}
		}
		while (!WaitingEmergency.isEmpty())
		{
			Mission* mission;
			WaitingEmergency.peek(mission);
			if (!AvailableEmergency.isEmpty())
			{
				Rover* rover;
				AvailableEmergency.peek(rover);
				mission->AssignRover(rover);
				WaitingEmergency.dequeue(mission);
				mission->setWD(Days);
				int CD = mission->GetCompletionDay();
				InExecutionMissions.enqueuelowest(mission, CD);
				AvailableEmergency.dequeue(rover);
			}
			else if (!AvailablePolar.isEmpty())
			{
				Rover* rover;
				AvailablePolar.peek(rover);
				mission->AssignRover(rover);
				WaitingEmergency.dequeue(mission);
				mission->setWD(Days);
				int CD = mission->GetCompletionDay();
				InExecutionMissions.enqueuelowest(mission, CD);
				AvailablePolar.dequeue(rover);
			}
			else if (!MaintainanceRovers.isEmpty())
			{
				Rover* rover;
				MaintainanceRovers.peek(rover);
				rover->setspeed(rover->getspeed() / 2);
				mission->AssignRover(rover);
				WaitingEmergency.dequeue(mission);
				mission->setWD(Days);
				int CD = mission->GetCompletionDay();
				InExecutionMissions.enqueuelowest(mission, CD);
				MaintainanceRovers.dequeue(rover);
			}
			else
			{
				break;
			}
		}
		while (!WaitingPolar.isEmpty())
		{
			Mission* mission;
			WaitingPolar.peek(mission);
			if (!AvailablePolar.isEmpty())
			{
				Rover* rover;
				AvailablePolar.peek(rover);
				mission->AssignRover(rover);
				WaitingPolar.dequeue(mission);
				mission->setWD(Days);
				int CD = mission->GetCompletionDay();
				InExecutionMissions.enqueuelowest(mission, CD);
				AvailablePolar.dequeue(rover);
			}
			else if (!MaintainanceRovers.isEmpty())
			{
				Rover* rover;
				MaintainanceRovers.peek(rover);
				rover->setspeed(rover->getspeed() / 2);
				mission->AssignRover(rover);
				WaitingPolar.dequeue(mission);
				mission->setWD(Days);
				int CD = mission->GetCompletionDay();
				InExecutionMissions.enqueuelowest(mission, CD);
				MaintainanceRovers.dequeue(rover);
			}
			else
			{
				break;
			}
		}
		if (modofoperation == 1)
		{
			if (Days == 1)
			{
				printDailyData(Days);
			}
			else
			{
				InteractiveMode(Days);
			}
		}
		else if (modofoperation == 2)
		{
			if (Days == 1)
			{
				printDailyData(Days);
			}
			else
			{
				Step_by_Step_Mode(Days);
			}
		}
		Days++;
	}
	PrintOutputFile();
}

inline void MarsStation::printDailyData(int d)
{
	drawline();
	pUI->Output("Current Day:" + to_string(d));
	pUI->Output("\n");
	pUI->Output(to_string(WaitingEmergency.getcount() + WaitingPolar.getcount()) + " Waiting Missions: ");
	printPriorityMissions(WaitingEmergency);
	printMissions(WaitingPolar);
	pUI->Output("\n");
	drawline();
	pUI->Output(to_string(InExecutionMissions.getcount()) + " In-Execution Missions/Rovers: ");
	printInExecutionEmergency(InExecutionMissions);
	printInExecutionPolar(InExecutionMissions);
	pUI->Output("\n");
	drawline();
	pUI->Output(to_string(AvailableEmergency.getcount() + AvailablePolar.getcount()) + " Available Rovers: ");
	printRovers(AvailableEmergency);
	printRovers(AvailablePolar);
	pUI->Output("\n");
	drawline();
	pUI->Output(to_string(CheckupRovers.getcount()) + " In-Checkup Rovers: ");
	printCheckupEmergency(CheckupRovers);
	printCheckupPolar(CheckupRovers);
	pUI->Output("\n");
	drawline();
	if (!MaintainanceRovers.isEmpty())
	{
		pUI->Output(to_string(MaintainanceRovers.getcount()) + " In-Maintenance Rovers: ");
		printRovers(MaintainanceRovers);
		pUI->Output("\n");
		drawline();
	}
	pUI->Output(to_string(CompletedMissions.getcount()) + " Completed Missions: ");
	printCompletedEmergency(CompletedMissions);
	printCompletedPolar(CompletedMissions);
	pUI->Output("\n");
	drawline();
	if (!FailedMissions.isEmpty())
	{
		pUI->Output(to_string(FailedMissions.getcount()) + " Failed Missions: ");
		printCompletedEmergency(FailedMissions); //this function for completed missions but it can work for failed missions without problems
		printCompletedPolar(FailedMissions);
		pUI->Output("\n");
		drawline();
	}
}

inline void MarsStation::drawline()
{
	for (int i = 0; i < 100; i++)
	{
		pUI->Output("-");
	}
	pUI->Output("\n");
}

inline void MarsStation::printMissions(LinkedQueue<Mission*> M)
{
	Mission* m;
	int n = M.getcount();
	if (M.peek(m) && m->getType() == 'E')
	{
		pUI->Output("[");
		for (int i = 0; i < n; i++)
		{
			M.dequeue(m);
			pUI->Output(to_string(m->GetID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output("]");
	}
	else if (M.peek(m) && m->getType() == 'P')
	{
		pUI->Output("(");
		for (int i = 0; i < n; i++)
		{
			M.dequeue(m);
			pUI->Output(to_string(m->GetID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output(")");
	}
}

inline void MarsStation::printPriorityMissions(PriorityQueue<Mission*> M)
{
	Mission* m;
	int n = M.getcount();
	if (M.peek(m))
	{
		pUI->Output("[");
		for (int i = 0; i < n; i++)
		{
			M.dequeue(m);
			pUI->Output(to_string(m->GetID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output("]");
	}
}

inline void MarsStation::printRovers(PriorityQueue<Rover*> R)
{
	Rover* r;
	R.peek(r);
	int n = R.getcount();
	if (R.peek(r) && r->gettype() == 'E')
	{
		pUI->Output("[");
		for (int i = 0; i < n; i++)
		{
			R.dequeue(r);
			pUI->Output(to_string(r->getID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output("]");
	}
	else if (R.peek(r) && r->gettype() == 'P')
	{
		pUI->Output("(");
		for (int i = 0; i < n; i++)
		{
			R.dequeue(r);
			pUI->Output(to_string(r->getID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output(")");
	}
}

inline void MarsStation::printCheckupPolar(PriorityQueue<Rover*> R)
{
	LinkedQueue<Rover*> temp2;
	Rover* r;
	while (!R.isEmpty())
	{
		R.dequeue(r);
		if (r->gettype() == 'P')
		{
			temp2.enqueue(r);
		}
	}
	int n = temp2.getcount();
	if (!temp2.isEmpty())
	{
		pUI->Output("(");
		for (int i = 0; i < n; i++)
		{
			temp2.dequeue(r);
			pUI->Output(to_string(r->getID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output(")");
	}
}

inline void MarsStation::printCheckupEmergency(PriorityQueue<Rover*> R)
{
	LinkedQueue<Rover*> temp2;
	Rover* r;
	while (!R.isEmpty())
	{
		R.dequeue(r);
		if (r->gettype() == 'E')
		{
			temp2.enqueue(r);
		}
	}
	int n = temp2.getcount();
	if (!temp2.isEmpty())
	{
		pUI->Output("[");
		for (int i = 0; i < n; i++)
		{
			temp2.dequeue(r);
			pUI->Output(to_string(r->getID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output("]");
	}
}

inline void MarsStation::printInExecutionPolar(PriorityQueue<Mission*> temp1)
{
	LinkedQueue<Mission*> temp2;
	Mission* m;
	while (!temp1.isEmpty())
	{
		temp1.dequeue(m);
		if (m->getType() == 'P')
		{
			temp2.enqueue(m);
		}
	}
	int n = temp2.getcount();
	if (!temp2.isEmpty())
	{
		pUI->Output("(");
		for (int i = 0; i < n; i++)
		{
			temp2.dequeue(m);
			pUI->Output(to_string(m->GetID())+"/"+to_string(m->getRover()->getID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output(")");
	}
}

inline void MarsStation::printInExecutionEmergency(PriorityQueue<Mission*> temp1)
{
	LinkedQueue<Mission*> temp2;
	Mission* m;
	while (!temp1.isEmpty())
	{
		temp1.dequeue(m);
		if (m->getType() == 'E')
		{
			temp2.enqueue(m);
		}
	}
	int n = temp2.getcount();
	if (!temp2.isEmpty())
	{
		pUI->Output("[");
		for (int i = 0; i < n; i++)
		{
			temp2.dequeue(m);
			pUI->Output(to_string(m->GetID()) + "/" + to_string(m->getRover()->getID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output("]");
	}
}

inline void MarsStation::printCompletedPolar(LinkedQueue<Mission*> temp1)
{
	LinkedQueue<Mission*> temp2;
	Mission* m;
	while (!temp1.isEmpty())
	{
		temp1.dequeue(m);
		if (m->getType() == 'P')
		{
			temp2.enqueue(m);
		}
	}
	int n = temp2.getcount();
	if (!temp2.isEmpty())
	{
		pUI->Output("(");
		for (int i = 0; i < n; i++)
		{
			temp2.dequeue(m);
			pUI->Output(to_string(m->GetID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output(")");
	}
}

inline void MarsStation::printCompletedEmergency(LinkedQueue<Mission*> temp1)
{
	LinkedQueue<Mission*> temp2;
	Mission* m;
	while (!temp1.isEmpty())
	{
		temp1.dequeue(m);
		if (m->getType() == 'E')
		{
			temp2.enqueue(m);
		}
	}
	int n = temp2.getcount();
	if (!temp2.isEmpty())
	{
		pUI->Output("[");
		for (int i = 0; i < n; i++)
		{
			temp2.dequeue(m);
			pUI->Output(to_string(m->GetID()));
			if (i + 1 < n)
			{
				pUI->Output(", ");
			}
		}
		pUI->Output("]");
	}
}

inline void MarsStation::InteractiveMode(int d)
{
	bool s=pUI->waitPressEnter();
	if (s)
	{
		printDailyData(d);
	}
}

inline void MarsStation::Step_by_Step_Mode(int d)
{
	sleep_for(1s);
	printDailyData(d);
}

inline void MarsStation::PrintOutputFile()
{
	Mission* m;
	ofstream outfile;
	outfile.open("Output File.txt");
	int summissions = CompletedMissions.getcount();
	int sumPolarmissions = 0;
	int sumEmergencymissions = 0;
	int sumPolarrovers = AvailablePolar.getcount();
	int sumEmergencyrovers = AvailableEmergency.getcount();
	int sumrovers = sumPolarrovers + sumEmergencyrovers;
	int sumwait=0;
	int sumexecution = 0;
	outfile << "CD ID FD WD ED" << endl;
	while (!CompletedMissions.isEmpty())
	{
		CompletedMissions.dequeue(m);
		outfile << m->GetCompletionDay() << "  " << m->GetID() << "  " << m->GetFD() << "  " << m->GetWD() << "  " << m->GetED() << endl;
		sumwait += m->GetWD();
		sumexecution += m->GetED();
		if (m->getType() == 'P')
		{
			sumPolarmissions++;
		}
		else if (m->getType() == 'E')
		{
			sumEmergencymissions++;
		}
	}
	outfile << "-------------------------------------------------------------------------------------------" << endl << "-------------------------------------------------------------------------------------------" << endl;
	outfile << "Missions:" << summissions << " [P:" << sumPolarmissions << ",E:" << sumEmergencymissions << "]" << endl;
	outfile << "Rovers:" << sumrovers << " [P:" << sumPolarrovers << ",E:" << sumEmergencyrovers << "]" << endl;
	outfile << "Avg Wait= " << (float)sumwait / summissions << ", Avg Exec= " << (float)sumexecution / summissions;
	outfile.close();
}


