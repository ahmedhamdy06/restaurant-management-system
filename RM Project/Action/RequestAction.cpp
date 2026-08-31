#include "RequestAction.h"
#include "../Restaurant/Restaurant.h"

void RequestAction::Act(Restaurant* pRest)
{
	Order* pOrd = new Order(orderId, type, actionTimeStep, size, price);
	
	if (pOrd->isDelivery())
	{
		pOrd->setDistance(distance);
	}
	else if (pOrd->isDineIn())
	{
		pOrd->setCanShare(canShare);
		pOrd->setDineIn(seats, duration);
	}
	else
	{
		pOrd->setDineIn(0, 0);
		pOrd->setCanShare(false);
		pOrd->setDistance(0);
	}
	pRest->AddOrder(pOrd);
}

void RequestAction::print(std::ostream& out) const
{
	out << "[";

	switch (type)
	{
	case 0: out << "ODG"; break;
	case 1: out << "ODN"; break;
	case 2: out << "OT";  break;
	case 3: out << "OVC"; break;
	case 4: out << "OVG"; break;
	case 5: out << "OVN"; break;
	case 6: out << "COMBO"; break;
	}
	
	out << ", " << actionTimeStep << ", " << orderId << "]";
}