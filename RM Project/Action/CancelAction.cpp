#include "CancelAction.h"
#include "../Restaurant/Restaurant.h"

void CancelAction::Act(Restaurant* pRest)
{
	pRest->CancelOrder(orderId);
}

void CancelAction::print(std::ostream& out) const
{
	out << "(X, " << actionTimeStep << ", " << orderId << ")";
}