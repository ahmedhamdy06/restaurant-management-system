#pragma once
#include <iostream>
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"
class Chef;
class Scooter;
class Table;
class Order {
public :
	enum OrderStatus{PENDING, COOKING, READY, IN_SERVICE, FINISHED, CANCELLED};
	enum OrderType  {ODG, ODN, OT, OVC, OVG, OVN , COMBO };

	Order(int ID, OrderType T, int tq, int s, double p)
		: id(ID), type(T), size(s), price(p), canShare(false), seats(0), duration(0), distance(0),
		TQ(tq), TA(0), TS(0), TR(0), TF(0), Tserv(0), stat(PENDING),
		assignedTable(nullptr), chefCount(0), scooterCount(0)
	{
		for (int i = 0; i < 4; i++) {
			assignedChefs[i] = nullptr;
			assignedScooters[i] = nullptr;
		}
		if (T == COMBO) {
			isCombo = true;
			chefsNeeded = 4;     
			scootersNeeded = 2; 
		}
		else {
			isCombo = false;
			chefsNeeded = 1;
			scootersNeeded = 1;
		}
	}

	bool isDineIn() const {
		return (type == ODG || type == ODN);
	}
	bool isDelivery() const {
		return (type == OVC || type == OVG || type == OVN || type == COMBO);      //helper functions
	}
	bool isTakeaway() const {
		return (type == OT);
	}

	int getId() const { return id; }
	OrderType getType() const { return type; }
	OrderStatus getStatus() const { return stat; }
	int getSize() const { return size; }
	double getPrice() const { return price; }
	int getDistance() const { return distance; }
	int getSeats() const { return seats; }
	int getDuration() const { return duration; }
	bool isSharable() const { return canShare; }
	int getTA() const { return TA; }
	int getTR() const { return TR; }
	int getTF() const { return TF; }
	int getTS() const { return TS; }
	int getTQ() const { return TQ; }

	int getIdleTime1() const {
		return TA - TQ;
	}

	int getIdleTime2() const {
		return TS - TR;
	}

	int getIdleTime() const {
		return getIdleTime1() + getIdleTime2();  // Ti
	}

	int getCookPeriod() const {
		return TR - TA;   // Tc
	}

	int getWaitTime() const {
		return getIdleTime() + getCookPeriod(); // Tw
	}

	int getServTime() const {
		return TF - TS;
	}

	double getOVGPriority() const
	{
		if (size == 0 || distance == 0)
		{
			return 0;
		}
		
		return price / (size * distance);
	}
	int getScootersNeeded() const { return scootersNeeded; }
	int getChefsNeeded() const { return chefsNeeded; }

	void addChef(Chef* c) {
		if (chefCount < chefsNeeded) assignedChefs[chefCount++] = c;
	}
	int getChefCount() const { return chefCount; }
	Chef* getChef(int index) const {
		if (index >= 0 && index < chefCount) return assignedChefs[index];
		return nullptr;
	}
	void clearChefs() { chefCount = 0; }

	void addScooter(Scooter* s) {
		if (scooterCount < scootersNeeded) assignedScooters[scooterCount++] = s;
	}
	int getScooterCount() const { return scooterCount; }
	Scooter* getScooter(int index) const {
		if (index >= 0 && index < scooterCount) return assignedScooters[index];
		return nullptr;
	}
	void clearScooters() { scooterCount = 0; }

	Table* getTable() const {
		return assignedTable;
	}

	void setStatus(OrderStatus s) { stat = s; }
	void setTable(Table* t) { assignedTable = t; }
	void setTA(int time) { TA = time; }
	void setTR(int time) { TR = time; }
	void setTS(int time) { TS = time; }
	void setTserv(int time) { Tserv = time; }
	void setTF(int time) { TF = time; }
	void setDistance(int d) { distance = d; }
	void setDineIn(int s, int dur) { seats = s; duration = dur; }
	void setCanShare(bool shr) { canShare = shr; }

	void print(std::ostream& out) const
	{
		if (getStatus() == Order::IN_SERVICE)
		{
			out << "[" << getId() << ", ";
			if (isDineIn() && getTable())
			{
				out << 'T' << getTable()->getId();
			}
			else if (isDelivery() && getScooterCount() > 0)
			{
				out << 'S' << getScooter(0)->getID();
			}
			out << "]";
		}
		else if (getStatus() == Order::COOKING)
		{
			out << "[" << getId() << ", ";

			if (getChefCount() > 0)
				out << getChef(0)->getID() << "]";
			else
				out << "NoChef]";
		}
		else
		{
			out << getId();
		}
	}
	void setAsCombo(int cRequired, int sRequired) {
		isCombo = true;
		chefsNeeded = cRequired;
		scootersNeeded = sRequired;
	}

	bool getIsCombo() const { return isCombo; }

	void setRemainingDistance(int dist) { remainingDistance = dist; }

	void setDistanceToMeetScooter(int dist) { distanceDone = dist; }

	int getRemainingDistance() const { return remainingDistance; }
	
	int getDistanceToMeetScooter() const { return distanceDone; }

	bool getFailedBefore() { return failedBefore; }

	void setFailedBefore(bool val) { failedBefore = val; }

private:
	OrderStatus stat;
	OrderType type;

	int chefsNeeded = 1;
	int chefsFinished = 0 ;
	int scootersNeeded = 1;
	int scootersFinished = 0;
	int remainingDistance = 0;
	int distanceDone = 0;
	bool isCombo = false;
	bool isOverwait = false;
	bool failedBefore = false;
	int id;
	int size;
	double price;

	int TQ, TA, TR, TS, Tserv, TF;

	int seats;       //for Dine_in
	int duration;
	bool canShare;

	int distance;  //for Delivery

	Chef* assignedChefs[4]; 
	int chefCount = 0;

	Scooter* assignedScooters[4]; 
	int scooterCount = 0;
	Table* assignedTable = nullptr;
};

inline std::ostream& operator<<(std::ostream& out, const Order* ord) {
	if (ord != nullptr) {
		ord->print(out);
	}
	return out;
}
