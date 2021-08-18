#pragma once
class Rover
{
private:
	int ID;
	float speed;
	char Type;
	int missions_checkup;
	int missions_done;
	int checkup_duration;
	int extraction_Day;   
	int maintenance_Duration;
	int extraction_after_maintenance;
public:
	Rover(char t,int sd,int id,int mschup,int chdur);
	void setID(int id);
	int getID();
	float getspeed();
	char gettype();
	int get_missions_checkup();
	int get_checkup_duration();
	void set_missions_done(int md);
	void Increment_missions();
	int get_missions_done();
	void set_extraction_Day(int eD);
	int get_extraction_Day();
	void setspeed(float sp);
	int get_main_Duration();
	void set_main_Duration(int mt_dur);
	void set_extraction_main(int main_ed);
	int get_extraction_main();
};
Rover::Rover(char t,int sd,int id,int mschup,int chdur)
{
	missions_done = 0;
	Type = t;
	speed = sd;
	setID(id);
	missions_checkup = mschup;
	checkup_duration = chdur;
	maintenance_Duration = 10;
}

inline void Rover::setID(int id)
{
	ID = id;
}

inline int Rover::getID()
{
	return ID;
}

inline float Rover::getspeed()
{
	return speed;
}

inline char Rover::gettype()
{
	return Type;
}

inline int Rover::get_missions_checkup()
{
	return missions_checkup;
}

inline int Rover::get_checkup_duration()
{
	return checkup_duration;
}

inline void Rover::set_missions_done(int md)
{
	missions_done = md;
}

inline void Rover::Increment_missions()
{
	missions_done++;
}

inline int Rover::get_missions_done()
{
	return missions_done;
}

inline void Rover::set_extraction_Day(int eD)
{
	extraction_Day = eD;
}

inline int Rover::get_extraction_Day()
{
	return extraction_Day;
}

inline void Rover::setspeed(float sp)
{
	speed = sp;
}

inline int Rover::get_main_Duration()
{
	return maintenance_Duration;
}

inline void Rover::set_main_Duration(int mt_dur)
{
	maintenance_Duration = mt_dur;
}

inline void Rover::set_extraction_main(int main_ed)
{
	extraction_after_maintenance = main_ed;
}

inline int Rover::get_extraction_main()
{
	return extraction_after_maintenance;
}



