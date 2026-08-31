#pragma once
#include "../Entities/Order.h"

class Restaurant;

class Action
{
protected:
	int orderId;
	int actionTimeStep;
public:
	Action(int time, int id):actionTimeStep(time), orderId(id) { }
	virtual ~Action() {}

	int getActionTime() const { return actionTimeStep; }
	int getOrderId() const { return orderId; }
	
	virtual void print(std::ostream& out) const = 0;
	virtual void Act(Restaurant* pRs) = 0;
};

inline std::ostream& operator<<(std::ostream& out, const Action* act) {
	if (act != nullptr) {
		act->print(out);
	}
	return out;
}