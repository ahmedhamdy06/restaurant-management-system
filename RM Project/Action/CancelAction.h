#pragma once
#include "Action.h"

class CancelAction : public Action
{
public:
	CancelAction(int time, int id): Action(time, id) {}

	virtual void print(std::ostream& out) const;
	virtual void Act(Restaurant* pRest);
};