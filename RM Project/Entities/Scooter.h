#pragma once
#include <iostream>
class Scooter {
public:
	enum ScooterStatus {AVAILABLE, IN_SERVICE, BACK, IN_MAINTENANCE, FAILED};
	Scooter(int ID, int s, int dur, int l) :id(ID), speed(s), main_dur(dur), stat(AVAILABLE), Maintain_Count(0), mainOrdsLimit(l),total_distance(0),returnTime(0), startTime(0), isBroken(false)
	{
	}
	int getID() const { return id; }
	int getSpeed() const { return speed; }
	int getMainDur() const { return main_dur; }
	ScooterStatus getStatus() const { return stat; }
	int getTripsCount() const { return Maintain_Count; }
	int getMainOrdsLimit() const { return mainOrdsLimit; }

	void setStatus(ScooterStatus s) { stat = s; }
	void setReturnTime(int retTime) { returnTime = retTime; }
	void incrementTrips() { Maintain_Count++; }      //helper functions
	void resetTrips() { Maintain_Count = 0; }  

	void addDistance(double d) { total_distance += d; }
	double getTotalDistance() const { return total_distance; }

	int getReturnTime() const { return returnTime; }
	void print(std::ostream& out) const {
		out << id;
	}

	void setstartTime(int time)
	{
		startTime = time;
	}

	int getstartTime() const
	{
		return startTime;
	}

	void setBroken(bool faulty)
	{
		isBroken = faulty;
	}

	bool isScooterBroken() const
	{
		return isBroken;
	}

private:
	int id;
	ScooterStatus stat;
	int speed;
	double total_distance;
	int returnTime;
	int Maintain_Count;  //counter before MAINTENANCE period
	int main_dur;        //MAINTENANCE duration
	int mainOrdsLimit;   //maximum orders before MAINTENANCE

	int startTime;
	bool isBroken;
};

inline std::ostream& operator<<(std::ostream& out, const Scooter* sco) {
	if (sco != nullptr) {
		sco->print(out);
	}
	return out;
}
