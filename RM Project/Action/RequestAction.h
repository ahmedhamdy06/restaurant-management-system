#pragma once
#include "Action.h"

class RequestAction : public Action
{
private:
	int distance; //Delivary

	int seats;   //Dine_in
	int duration;
	bool canShare;

	Order::OrderType type;
	int size;
	double price;

public:
	RequestAction(int t, int id, Order::OrderType type, int s, double p, int dist) :Action(t, id), type(type),
		size(s), price(p), distance(dist), seats(0), duration(0), canShare(false) { } //Delivary Constructor

	RequestAction(int t, int id, Order::OrderType type, int s, double p, int st, int dur, bool shr) :Action(t, id), type(type),
		size(s), price(p), distance(0), seats(st), duration(dur), canShare(shr) { } //Dine_in Constructor

	RequestAction(int t, int id, Order::OrderType type, int s, double p) :Action(t, id), type(type),
		size(s), price(p), distance(0), seats(0), duration(0), canShare(false) { } //Take_Away Constructor

	virtual void print(std::ostream &out) const;
	virtual void Act(Restaurant* pRs);
};