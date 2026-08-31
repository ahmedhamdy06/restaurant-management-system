#include "Restaurant.h"
#include <fstream>
Restaurant::Restaurant()
{
	currentTimeStep = 0;
	TH = 0;
	TotalCN = 0;
	TotalCS = 0;
	TotalScooters = 0;
	TotalChefsBusyTime = 0;
	TotalScootersBusyTime = 0;
	TotalScooterFailures = 0;
	MaxTableCapacity = 0;
	pUi = new UI();
}

void Restaurant::IncrementTimeStep()
{
	currentTimeStep++;
}

int Restaurant::GetCurrentTimeStep() const
{
	return currentTimeStep;
}

Restaurant::~Restaurant()
{
	Order* o;
	Chef* c;
	Scooter* s;
	Table* t;
	Action* a;
	double pri;

	// Actions
	while (Actions.dequeue(a)) delete a;

	// Pending
	while (PEND_ODG.dequeue(o)) delete o;
	while (PEND_ODN.dequeue(o)) delete o;
	while (PEND_OT.dequeue(o)) delete o;
	while (PEND_OVN.dequeue(o)) delete o;
	while (PEND_OVC.dequeue(o)) delete o;
	while (PEND_OVG.dequeue(o, pri)) delete o;

	// Chefs
	while (Free_CS.dequeue(c)) delete c;
	while (Free_CN.dequeue(c)) delete c;

	// Cooking
	while (Cooking_Orders.dequeue(o, pri)) delete o;

	// Ready
	while (RDY_OT.dequeue(o)) delete o;
	while (RDY_OV.dequeue(o)) delete o;
	while (RDY_OD.dequeue(o)) delete o;

	// In service
	while (InServ_Orders.dequeue(o, pri)) delete o;

	// Finished / Cancelled
	while (Finished_orders.pop(o)) delete o;
	while (Cancelled_orders.dequeue(o)) delete o;

	// Scooters
	while (Free_Scooters.dequeue(s, pri)) delete s;
	while (Back_Scooters.dequeue(s, pri)) delete s;
	while (Maint_Scooters.dequeue(s)) delete s;

	// Tables
	while (Free_Tables.dequeue(t, pri)) delete t;
	while (Busy_Sharable_Tables.dequeue(t, pri)) delete t;
	while (Busy_No_Share_Tables.dequeue(t, pri)) delete t;
	
	// Combo
	while (PEND_COMBO.dequeue(o, pri)) delete o;

	// Overwait
	while (Overwait_OVG.dequeue(o, pri)) delete o;
	
	//Rescue
	while (Rescue_Orders.dequeue(o, pri)) delete o;

	delete pUi;
}

void Restaurant::AddOrder(Order* o)
{
	if (!o) return;

	switch (o->getType())
	{
	case Order::ODG:
		PEND_ODG.enqueue(o);
		break;

	case Order::ODN:
		PEND_ODN.enqueue(o);
		break;

	case Order::OT:
		PEND_OT.enqueue(o);
		break;

	case Order::OVN:
		PEND_OVN.enqueue(o);
		break;

	case Order::OVC:
		PEND_OVC.enqueue(o);
		break;

	case Order::OVG:
		PEND_OVG.enqueue(o, o->getOVGPriority());
		break;

	case Order::COMBO:
		PEND_COMBO.enqueue(o, o->getOVGPriority());
		break;
	}
}

void Restaurant::CancelOrder(int id)
{
	Order* removed = PEND_OVC.cancelOrder(id);

	if (removed)
	{
		removed->setStatus(Order::CANCELLED);
		Cancelled_orders.enqueue(removed);
		return;
	}

	removed = RDY_OV.cancelOrder(id);

	if (removed)
	{
		removed->setStatus(Order::CANCELLED);
		Cancelled_orders.enqueue(removed);
		return;
	}

	removed = Cooking_Orders.cancelOrder(id);

	if (removed)
	{
		removed->setStatus(Order::CANCELLED);

		for (int i = 0; i < removed->getChefCount(); i++)
		{
			Chef* pChef = removed->getChef(i);
			if (pChef)
			{
				pChef->setStatus(Chef::AVAILABLE);
				if (pChef->getChefType() == Chef::CN) Free_CN.enqueue(pChef);
				else Free_CS.enqueue(pChef);
			}
		}
		removed->clearChefs();
		Cancelled_orders.enqueue(removed);
		return;
	}
}

int Restaurant::GetActionCount() const
{
	return Actions.getCount();
}

int Restaurant::GetPendingODGCount() const
{
	return PEND_ODG.getCount();
}
int Restaurant::GetPendingODNCount() const
{
	return PEND_ODN.getCount();
}
int Restaurant::GetPendingOTCount() const
{
	return PEND_OT.getCount();
}
int Restaurant::GetPendingOVNCount() const
{
	return PEND_OVN.getCount();
}
int Restaurant::GetPendingOVCCount() const
{
	return PEND_OVC.getCount();
}
int Restaurant::GetPendingOVGCount() const
{
	return PEND_OVG.getCount();
}

int Restaurant::GetFreeCNCount() const
{
	return Free_CN.getCount();
}
int Restaurant::GetFreeCSCount() const
{
	return Free_CS.getCount();
}

int Restaurant::GetCookingCount() const
{
	return Cooking_Orders.getCount();
}
int Restaurant::GetReadyOTCount() const
{
	return RDY_OT.getCount();
}
int Restaurant::GetReadyOVCount() const
{
	return RDY_OV.getCount();
}
int Restaurant::GetReadyODCount() const
{
	return RDY_OD.getCount();
}

int Restaurant::GetInServiceCount() const
{
	return InServ_Orders.getCount();
}
int Restaurant::GetFinishedCount() const
{
	return Finished_orders.getCount();
}
int Restaurant::GetCancelledCount() const
{
	return Cancelled_orders.getCount();
}

int Restaurant::GetFreeScootersCount() const
{
	return Free_Scooters.getCount();
}
int Restaurant::GetBackScootersCount() const
{
	return Back_Scooters.getCount();
}
int Restaurant::GetMaintScootersCount() const
{
	return Maint_Scooters.getCount();
}

int Restaurant::GetFreeTablesCount() const
{
	return Free_Tables.getCount();
}
int Restaurant::GetBusySharableTablesCount() const
{
	return Busy_Sharable_Tables.getCount();
}
int Restaurant::GetBusyNoShareTablesCount() const
{
	return Busy_No_Share_Tables.getCount();
}

void Restaurant::ReadInputFile(string fileName)
{
	ifstream file(fileName);
	if (!file.is_open()) return;

	int CN, CS, CN_Speed, CS_Speed;
	file >> CN >> CS;
	file >> CN_Speed >> CS_Speed;

	TotalCS = CS; TotalCN = CN;

	for (int i = 0; i < CN; i++)
	{
		Free_CN.enqueue(new Chef(i + 1, Chef::CN, CN_Speed));
	}
	for (int i = 0; i < CS; i++)
	{
		Free_CS.enqueue(new Chef(i + CN + 1, Chef::CS, CS_Speed));
	}

	int S_Count, S_Speed;
	file >> S_Count >> S_Speed;

	TotalScooters = S_Count;

	int Main_Ords, Main_Dur;
	file >> Main_Ords >> Main_Dur;

	for (int i = 0; i < S_Count; i++)
	{
		Free_Scooters.enqueue(new Scooter(i + 1, S_Speed, Main_Dur, Main_Ords), 0);
	}

	int Total_no_of_Tables;
	file >> Total_no_of_Tables;

	int tableId = 1;
	int read = 0;
	while (read < Total_no_of_Tables)
	{
		int count, capacity;
		file >> count >> capacity;
		if (capacity > MaxTableCapacity) MaxTableCapacity = capacity;

		for (int j = 0; j < count; j++)
		{
			Free_Tables.enqueue(new Table(tableId++, capacity), -capacity);
		}
		read += count;
	}

	file >> TH;

	int M;
	file >> M;

	for (int i = 0; i < M; i++)
	{
		char actionType;
		file >> actionType;

		if (actionType == 'Q')
		{
			string typeStr;
			int Tq, id, size, seats, dur, dist;
			double price;
			char share;

			file >> typeStr >> Tq >> id >> size >> price;

			Order::OrderType type;
			if (typeStr == "ODG")      type = Order::ODG;
			else if (typeStr == "ODN") type = Order::ODN;
			else if (typeStr == "OT")  type = Order::OT;
			else if (typeStr == "OVN") type = Order::OVN;
			else if (typeStr == "OVC") type = Order::OVC;
			else if (typeStr == "OVG") type = Order::OVG;
			else if (typeStr == "COMBO") type = Order::COMBO;

			if (type == Order::ODG || type == Order::ODN)
			{
				file >> seats >> dur >> share;
				bool canShare = (share == 'Y' || share == 'y');
				Actions.enqueue(new RequestAction(Tq, id, type, size, price, seats, dur, canShare));
			}
			else if (type == Order::OVG || type == Order::OVN || type == Order::OVC || type == Order::COMBO)
			{
				file >> dist;
				Actions.enqueue(new RequestAction(Tq, id, type, size, price, dist));
			}
			else
			{
				Actions.enqueue(new RequestAction(Tq, id, type, size, price));
			}
		}
		else if (actionType == 'X')
		{
			int Tq, id;
			file >> Tq >> id;
			Actions.enqueue(new CancelAction(Tq, id));
		}
	}
	file.close();
}

void Restaurant::AssignToChefs()
{
	Order* pCombo = nullptr;
	double comboPri;
	while (PEND_COMBO.peek(pCombo, comboPri))
	{
		if (GetFreeCSCount() >= 1)
		{
			PEND_COMBO.dequeue(pCombo, comboPri);

			Chef* pCS;
			Free_CS.dequeue(pCS);
			pCombo->addChef(pCS);
			pCS->setStatus(Chef::BUSY);

			Chef* pExtra;
			while (pCombo->getChefCount() < 4)
			{
				if (Free_CN.dequeue(pExtra)) pCombo->addChef(pExtra);
				else if (Free_CS.dequeue(pExtra)) pCombo->addChef(pExtra);
				else break;

				pCombo->getChef(pCombo->getChefCount() - 1)->setStatus(Chef::BUSY);
			}

			pCombo->setTA(GetCurrentTimeStep());
			pCombo->setStatus(Order::COOKING);

			int totalSpeed = 0;
			for (int i = 0; i < pCombo->getChefCount(); i++)
				totalSpeed += pCombo->getChef(i)->getSpeed();

			int cookPeriod = (pCombo->getSize() + totalSpeed - 1) / totalSpeed;
			pCombo->setTR(pCombo->getTA() + cookPeriod);

			Cooking_Orders.enqueue(pCombo, -pCombo->getTR());
		}
		else {
			break;
		}
	}
	Chef* pChef = nullptr;
	Order* pOrd = nullptr;
	double pri;
	// ODG -> CS only
	while (PEND_ODG.peek(pOrd) && Free_CS.peek(pChef))
	{
		PEND_ODG.dequeue(pOrd);
		Free_CS.dequeue(pChef);
		StartCooking(pOrd, pChef);
	}
	//ODN -> CN then CS
	while (PEND_ODN.peek(pOrd))
	{
		if (Free_CN.peek(pChef))
		{
			PEND_ODN.dequeue(pOrd);
			Free_CN.dequeue(pChef);
			StartCooking(pOrd, pChef);
		}
		else if (Free_CS.peek(pChef))
		{
			PEND_ODN.dequeue(pOrd);
			Free_CS.dequeue(pChef);
			StartCooking(pOrd, pChef);
		}
		else
			break;
	}
	//OT -> CN only
	while (PEND_OT.peek(pOrd) && Free_CN.peek(pChef))
	{
		PEND_OT.dequeue(pOrd);
		Free_CN.dequeue(pChef);
		StartCooking(pOrd, pChef);
	}
	//OVG -> CS only
	while (PEND_OVG.peek(pOrd, pri) && Free_CS.peek(pChef)) {
		PEND_OVG.dequeue(pOrd, pri);
		Free_CS.dequeue(pChef);
		StartCooking(pOrd, pChef);
	}
	//OVC -> CN then CS 
	while (PEND_OVC.peek(pOrd)) {
		if (Free_CN.peek(pChef))
		{
			PEND_OVC.dequeue(pOrd);
			Free_CN.dequeue(pChef);
			StartCooking(pOrd, pChef);
		}
		else if (Free_CS.peek(pChef))
		{
			PEND_OVC.dequeue(pOrd);
			Free_CS.dequeue(pChef);
			StartCooking(pOrd, pChef);
		}
		else
			break;
	}
	//OVN -> CN only
	while (PEND_OVN.peek(pOrd) && Free_CN.peek(pChef)) {
		PEND_OVN.dequeue(pOrd);
		Free_CN.dequeue(pChef);
		StartCooking(pOrd, pChef);
	}
}

void Restaurant::StartCooking(Order* pOrd, Chef* pChef)
{
	pOrd->addChef(pChef);
	pOrd->setStatus(Order::COOKING);
	pChef->setStatus(Chef::BUSY);

	pOrd->setTA(GetCurrentTimeStep());
	int Cooking_time = (pOrd->getSize() + pChef->getSpeed() - 1) / (pChef->getSpeed());
	pOrd->setTR(pOrd->getTA() + Cooking_time);
	Cooking_Orders.enqueue(pOrd, -pOrd->getTR());
}

void Restaurant::MoveToReady()
{
	Order* pOrd = nullptr;
	double pri;

	while (Cooking_Orders.peek(pOrd, pri))
	{
		if (pOrd->getTR() <= GetCurrentTimeStep())
		{
			Cooking_Orders.dequeue(pOrd, pri);
			pOrd->setStatus(Order::READY);

			for (int i = 0; i < pOrd->getChefsNeeded(); i++)
			{
				Chef* pChef = pOrd->getChef(i);
				if (pChef)
				{
					TotalChefsBusyTime += pOrd->getCookPeriod();
					pChef->setStatus(Chef::AVAILABLE);
					if (pChef->getChefType() == Chef::CN) Free_CN.enqueue(pChef);
					else Free_CS.enqueue(pChef);
				}
			}
			if (pOrd->getType() == Order::COMBO || pOrd->isDelivery())
			{
				RDY_OV.enqueue(pOrd);
			}
			else if (pOrd->isTakeaway())
			{
				RDY_OT.enqueue(pOrd);
				pOrd->setTS(pOrd->getTR());
			}
			else if (pOrd->isDineIn())
			{
				RDY_OD.enqueue(pOrd);
			}
		}
		else
		{
			break;
		}
	}
}

bool Restaurant::AssignDineIn(Order* o)
{
	Table* table = nullptr;
	int neededSeats = o->getSeats();
	bool allowsShare = o->isSharable();

	if (allowsShare)
	{
		table = Busy_Sharable_Tables.getBest(neededSeats);
		if (table)
		{
			table->occupySeats(neededSeats, allowsShare);

			if (table->getStatus() == Table::BUSY_NO_SHARE)
			{
				Busy_No_Share_Tables.enqueue(table, 0);
			}
			else if (table->getStatus() == Table::BUSY_SHARABLE)
			{
				Busy_Sharable_Tables.enqueue(table, -table->getAvailableSeats());
			}

			o->setTS(GetCurrentTimeStep());
			int finishTime = GetCurrentTimeStep() + o->getDuration();
			o->setTF(finishTime);

			o->setStatus(Order::IN_SERVICE);
			o->setTable(table);
			InServ_Orders.enqueue(o, -o->getTF());

			return true;
		}
		else
		{
			table = Free_Tables.getBest(neededSeats);

			if (table)
			{
				table->occupySeats(neededSeats, allowsShare);

				if (table->getStatus() == Table::BUSY_NO_SHARE)
				{
					Busy_No_Share_Tables.enqueue(table, 0);
				}
				else if (table->getStatus() == Table::BUSY_SHARABLE)
				{
					Busy_Sharable_Tables.enqueue(table, -table->getAvailableSeats());
				}

				o->setTS(GetCurrentTimeStep());
				int finishTime = GetCurrentTimeStep() + o->getDuration();
				o->setTF(finishTime);

				o->setStatus(Order::IN_SERVICE);
				o->setTable(table);
				InServ_Orders.enqueue(o, -o->getTF());

				return true;
			}
		}
	}

	table = Free_Tables.getBest(neededSeats);

	if (table)
	{
		table->occupySeats(neededSeats, allowsShare);

		if (table->getStatus() == Table::BUSY_NO_SHARE)
		{
			Busy_No_Share_Tables.enqueue(table, 0);
		}
		else if (table->getStatus() == Table::BUSY_SHARABLE)
		{
			Busy_Sharable_Tables.enqueue(table, -table->getAvailableSeats());
		}

		o->setTS(GetCurrentTimeStep());
		int finishTime = GetCurrentTimeStep() + o->getDuration();
		o->setTF(finishTime);

		o->setStatus(Order::IN_SERVICE);
		o->setTable(table);
		InServ_Orders.enqueue(o, -o->getTF());

		return true;
	}

	return false;
}

void Restaurant::AssignToTables()
{
	int n = RDY_OD.getCount();
	LinkedQueue<Order*> temp;

	Order* o;

	for (int i = 0; i < n; i++)
	{
		RDY_OD.dequeue(o);

		if (o->getSeats() > MaxTableCapacity) {
			o->setStatus(Order::CANCELLED);
			Cancelled_orders.enqueue(o);
			continue;
		}

		if (!AssignDineIn(o))
		{
			temp.enqueue(o);
		}
	}

	while (!temp.isEmpty())
	{
		Order* x;
		temp.dequeue(x);
		RDY_OD.enqueue(x);
	}
}

void Restaurant::AssignToScooters()
{
	Order* pOrd = nullptr;
	double pri;

	while (!Rescue_Orders.isEmpty())
	{
		Rescue_Orders.peek(pOrd, pri);
		if (AssignRescueScooter(pOrd)) 
			Rescue_Orders.dequeue(pOrd, pri);
		else break;
	}

	while (!Overwait_OVG.isEmpty())
	{
		Overwait_OVG.peek(pOrd, pri);
		if (AssignScooter(pOrd))
		{
			Overwait_OVG.dequeue(pOrd, pri);
		}
		else break;
	}

	LinkedQueue<Order*> tempCombo;
	LinkedQueue<Order*> tempOVC;
	LinkedQueue<Order*> tempOthers;

	while (RDY_OV.dequeue(pOrd))
	{
		if (pOrd->getType() == Order::COMBO)
			tempCombo.enqueue(pOrd);
		else if (pOrd->getType() == Order::OVC)
			tempOVC.enqueue(pOrd);
		else
			tempOthers.enqueue(pOrd);
	}
	while (!tempCombo.isEmpty())
	{
		tempCombo.peek(pOrd);

		if (AssignScooter(pOrd))
		{
			tempCombo.dequeue(pOrd);
		}
		else break;
	}
	while (!tempOVC.isEmpty())
	{
		tempOVC.peek(pOrd);

		if(AssignScooter(pOrd)) 			
				tempOVC.dequeue(pOrd);
		else break;
	}
	while (!tempOthers.isEmpty())
	{
		tempOthers.peek(pOrd);
	
		if (AssignScooter(pOrd))
		{
			tempOthers.dequeue(pOrd);
		}
		else break;
	}
	while (tempCombo.dequeue(pOrd))  RDY_OV.enqueue(pOrd);
	while (tempOVC.dequeue(pOrd))    RDY_OV.enqueue(pOrd);
	while (tempOthers.dequeue(pOrd)) RDY_OV.enqueue(pOrd);
}

bool Restaurant::AssignScooter(Order* o)
{
	int needed = o->getScootersNeeded();
	if (GetFreeScootersCount() < needed) return false;

	for (int i = 0; i < needed; i++) {
		Scooter* s = nullptr; double p;
		Free_Scooters.dequeue(s, p);
		o->addScooter(s);
		s->setStatus(Scooter::IN_SERVICE);
		s->incrementTrips();
		s->addDistance(2 * o->getDistance());
		int deliveryTime = (o->getDistance() + s->getSpeed() - 1) / s->getSpeed();
		s->setReturnTime(GetCurrentTimeStep() + (2 * deliveryTime));
		s->setstartTime(GetCurrentTimeStep());
	}

	o->setTS(GetCurrentTimeStep());
	o->setStatus(Order::IN_SERVICE);
	int deliveryTime = (o->getDistance() + o->getScooter(0)->getSpeed() - 1) / o->getScooter(0)->getSpeed();
	int finishTime = GetCurrentTimeStep() + deliveryTime;
	o->setTF(finishTime);
	InServ_Orders.enqueue(o, -finishTime);
	return true;
}

void Restaurant::CheckInServiceOrders()
{
	Order* pOrd = nullptr;
	double pri;

	while (InServ_Orders.peek(pOrd, pri))
	{
		if (pOrd->getTF() <= GetCurrentTimeStep())
		{
			InServ_Orders.dequeue(pOrd, pri);
			pOrd->setStatus(Order::FINISHED);

			if (pOrd->isDineIn())
			{
				Table* pTable = pOrd->getTable();
				if (pTable)
				{
					Table* removed = Busy_No_Share_Tables.removeTable(pTable->getId());
					if (removed == nullptr)
					{
						removed = Busy_Sharable_Tables.removeTable(pTable->getId());
					}
					pTable->releaseSeats(pOrd->getSeats());
				}

				if (pTable->getAvailableSeats() == pTable->getCapacity())
				{
					pTable->setStatus(Table::FREE);
					Free_Tables.enqueue(pTable, -pTable->getCapacity());
				}
				else
				{
					Busy_Sharable_Tables.enqueue(pTable, -pTable->getAvailableSeats());
				}
			}
			else if (pOrd->isDelivery())
			{
				for (int i = 0; i < pOrd->getScooterCount(); i++)
				{
					Scooter* pScooter = pOrd->getScooter(i);
					if (pScooter)
					{
						pScooter->setStatus(Scooter::BACK);
						Back_Scooters.enqueue(pScooter, -pScooter->getReturnTime());
					}
				}
			}

			Finished_orders.push(pOrd);
		}
		else
		{
			break;
		}
	}
}

void Restaurant::CheckScooters()
{
	Scooter* pScooter = nullptr;
	double pri;

	while (Back_Scooters.peek(pScooter, pri))
	{
		if (-pri <= GetCurrentTimeStep())
		{
			Back_Scooters.dequeue(pScooter, pri);
			TotalScootersBusyTime += pScooter->getReturnTime() - pScooter->getstartTime();

			if (pScooter->getTripsCount() >= pScooter->getMainOrdsLimit())
			{
				pScooter->setStatus(Scooter::IN_MAINTENANCE);
				pScooter->setReturnTime(GetCurrentTimeStep() + pScooter->getMainDur());
				TotalScootersBusyTime += pScooter->getMainDur();
				Maint_Scooters.enqueue(pScooter);
			}
			else
			{
				pScooter->setStatus(Scooter::AVAILABLE);
				Free_Scooters.enqueue(pScooter, -pScooter->getTotalDistance());
			}
		}
		else
		{
			break;
		}
	}

	while (Maint_Scooters.peek(pScooter))
	{
		if (pScooter->getReturnTime() <= GetCurrentTimeStep())
		{
			Maint_Scooters.dequeue(pScooter);
			pScooter->resetTrips();
			pScooter->setStatus(Scooter::AVAILABLE);
			Free_Scooters.enqueue(pScooter, -pScooter->getTotalDistance());
		}
		else
		{
			break;
		}
	}
}

void Restaurant::CheckOverwaitOVG()
{
	Order* pOrd = nullptr;
	LinkedQueue<Order*> temp;

	while (RDY_OV.dequeue(pOrd))
	{
		if (pOrd->getType() == Order::OVG && (GetCurrentTimeStep() - pOrd->getTR() > TH))
		{
			double priority = (GetCurrentTimeStep() - pOrd->getTQ());
			Overwait_OVG.enqueue(pOrd, priority);
		}
		else
		{
			temp.enqueue(pOrd);
		}
	}

	while (temp.dequeue(pOrd))
	{
		RDY_OV.enqueue(pOrd);
	}
}

void Restaurant::CheckScooterFailures()
{
	Order* pOrd = nullptr;
	double pri;
	LinkedQueue<Order*> temp;

	while (InServ_Orders.dequeue(pOrd, pri))
	{
		if (pOrd->isDelivery() && !pOrd->getFailedBefore() && (rand() % 100 < 3))
		{
			TotalScooterFailures++;
			pOrd->setFailedBefore(true);

			Scooter* sFailed = pOrd->getScooter(0);

			int timePassed = GetCurrentTimeStep() - pOrd->getTS();
			int distanceDone = timePassed * sFailed->getSpeed();

			pOrd->setRemainingDistance(pOrd->getDistance() - distanceDone);
			pOrd->setDistanceToMeetScooter(distanceDone);

			sFailed->setStatus(Scooter::FAILED);
	
			Rescue_Orders.enqueue(pOrd, -pOrd->getTQ());
		}
		else
		{
			temp.enqueue(pOrd);
		}
	}
	while (temp.dequeue(pOrd))
		InServ_Orders.enqueue(pOrd, -pOrd->getTF());
}

bool Restaurant::AssignRescueScooter(Order* o)
{
	if (Free_Scooters.isEmpty()) return false;

	Scooter* sOld = o->getScooter(0);

	o->clearScooters();

	Scooter* sRescue = nullptr; double p;
	Free_Scooters.dequeue(sRescue, p);

	o->addScooter(sRescue);
	sRescue->setStatus(Scooter::IN_SERVICE);
	sRescue->setstartTime(GetCurrentTimeStep());

	int distToMeet = o->getDistanceToMeetScooter();
	int distLeft = o->getRemainingDistance();       

	int timeToMeet = (distToMeet + sRescue->getSpeed() - 1) / sRescue->getSpeed();
	int meetingTime = GetCurrentTimeStep() + timeToMeet;

	int timeToFinish = (distLeft + sRescue->getSpeed() - 1) / sRescue->getSpeed();
	int finishTime = meetingTime + timeToFinish;
	o->setTF(finishTime);

	int returnTimeRescue = finishTime + (o->getDistance() / sRescue->getSpeed());
	sRescue->setReturnTime(returnTimeRescue);

	sOld->setReturnTime(2 * timeToMeet + GetCurrentTimeStep());
	sOld->setStatus(Scooter::BACK);
	Back_Scooters.enqueue(sOld, -sOld->getReturnTime());

	o->setStatus(Order::IN_SERVICE);
	InServ_Orders.enqueue(o, -finishTime);

	return true;
}

void Restaurant::FinalizeTakeaway()
{
	Order* pOrd = nullptr;

	while (RDY_OT.peek(pOrd))
	{
		if (pOrd->getTR() + 1 <= GetCurrentTimeStep())
		{
			RDY_OT.dequeue(pOrd);

			pOrd->setTF(pOrd->getTS() + 1);

			pOrd->setStatus(Order::FINISHED);

			Finished_orders.push(pOrd);
		}
		else
		{
			break;
		}
	}
}

void Restaurant::ExecuteActions(int currentStep)
{
	Action* pAct = nullptr;
	while (Actions.peek(pAct))
	{
		if (pAct->getActionTime() <= currentStep)
		{
			Actions.dequeue(pAct);
			pAct->Act(this);
			delete pAct;
		}
		else
		{
			break;
		}
	}
}

void Restaurant::RunSimulation()
{
	pUi->readInterfaceMode();

	string inputFile, outputFile;
	pUi->readInputFileName(inputFile);
	ReadInputFile(inputFile);

	while (!Actions.isEmpty() ||
		!PEND_ODG.isEmpty() || !PEND_ODN.isEmpty() || !PEND_OT.isEmpty() ||
		!PEND_OVN.isEmpty() || !PEND_OVC.isEmpty() || !PEND_OVG.isEmpty() ||
		!PEND_COMBO.isEmpty() ||
		!Cooking_Orders.isEmpty() ||
		!RDY_OD.isEmpty() || !RDY_OT.isEmpty() || !RDY_OV.isEmpty() ||
		!InServ_Orders.isEmpty() || !Back_Scooters.isEmpty() ||
		!Maint_Scooters.isEmpty() || !Rescue_Orders.isEmpty() || !Overwait_OVG.isEmpty())
	{
		IncrementTimeStep();
		int step = GetCurrentTimeStep();

		ExecuteActions(step);

		CheckScooters();
		CheckInServiceOrders();
		MoveToReady();
		CheckScooterFailures();
		CheckOverwaitOVG();
		FinalizeTakeaway();

		AssignToChefs();
		AssignToTables();
		AssignToScooters();

		if (pUi->getInterfaceMode() == UI::INTERACTIVE)
		{
			pUi->PrintOutputScreen(step, Actions,
				PEND_ODG, PEND_ODN, PEND_OT, PEND_OVN, PEND_OVC, PEND_OVG,
				Free_CS, Free_CN, Cooking_Orders,
				RDY_OT, RDY_OD, RDY_OV,
				InServ_Orders, Finished_orders, Cancelled_orders,
				Free_Scooters, Back_Scooters, Maint_Scooters,
				Free_Tables, Busy_Sharable_Tables, Busy_No_Share_Tables, PEND_COMBO);

			cin.ignore(10000,'\n');
		}
	}

	pUi->readOutputFileName(outputFile);
	if (pUi->getInterfaceMode() == UI::SILENT)
	{
		cout << "Simulation Starts in Silent mode ...\n";
		cout << "Simulation ends, Output file created\n";
	}

	cout << "\nSimulation ended in " << GetCurrentTimeStep() << " timestep\n";

	GenerateOutputFile(outputFile);
}

void Restaurant::GenerateOutputFile(string fileName)
{
	ofstream outfile(fileName);
	if (!outfile.is_open()) return;

	outfile << "All Finished Orders\n-------------------\n";
	outfile << "N\tTF\tID\tTQ\tTA\tTR\tTS\tTI\tTC\tTw\tTserv\n";
	double totalFinished = Finished_orders.getCount();
	double totalCancelled = Cancelled_orders.getCount();
	double totalOrders = totalFinished + totalCancelled;

	int countODG = 0, countODN = 0, countOT = 0, countOVN = 0, countOVC = 0, countOVG = 0, countCombo = 0;
	double sumTi = 0, sumTc = 0, sumTw = 0, sumTserv = 0;
	double overwaitCount = 0;

	Order* pOrd;
	int i = 1;
	while (Finished_orders.pop(pOrd))
	{
		int Ti = pOrd->getIdleTime();
		int Tc = pOrd->getCookPeriod();
		int Tw = pOrd->getWaitTime();
		int Tserv = pOrd->getServTime();

		sumTi += Ti;
		sumTc += Tc;
		sumTw += Tw;
		sumTserv += Tserv;

		if (Tw > TH)
		{
			overwaitCount++;
		}

		switch (pOrd->getType())
		{
		case Order::ODG: countODG++; break;
		case Order::ODN: countODN++; break;
		case Order::OT: countOT++; break;
		case Order::OVN: countOVN++; break;
		case Order::OVC: countOVC++; break;
		case Order::OVG: countOVG++; break;
		case Order::COMBO: countCombo++; break;
		}

		outfile << i++ << "-\t" << pOrd->getTF() << '\t' << pOrd->getId() << '\t'
			<< pOrd->getTQ() << '\t' << pOrd->getTA() << '\t'
			<< pOrd->getTR() << '\t' << pOrd->getTS() << '\t'
			<< pOrd->getIdleTime() << '\t' << pOrd->getCookPeriod() << '\t'
			<< pOrd->getWaitTime() << '\t' << pOrd->getServTime() << '\n';
		delete pOrd;
	}
	outfile << "-------------------------------------------------------------------------------------\n\n";

	while (Cancelled_orders.dequeue(pOrd))
	{
		switch (pOrd->getType())
		{
		case Order::ODG: countODG++; break;
		case Order::ODN: countODN++; break;
		case Order::OT: countOT++; break;
		case Order::OVN: countOVN++; break;
		case Order::OVC: countOVC++; break;
		case Order::OVG: countOVG++; break;
		case Order::COMBO: countCombo++; break;
		}
		delete pOrd;
	}

	outfile << "Statistics\n----------\n";

	outfile << "1- Total Orders: " << totalOrders << " [ " << "ODN: " << countODN
		<< ", ODG: " << countODG << " , OT: " << countOT << ", OVN: " << countOVN
		<< ", OVC: " << countOVC << ", OVG: " << countOVG
		<< ", COMBO: " << countCombo << " ]\n\n";

	outfile << "2- Total Chefs: " << TotalCN + TotalCS << " [ " << "CN: " << TotalCN
		<< ", CS: " << TotalCS << " ]\n\n";

	outfile << "3- Total Scooters: " << TotalScooters << "\n\n";

	double finishedPerc = (totalOrders == 0) ? 0 : (totalFinished / totalOrders) * 100;
	double cancelledPerc = (totalOrders == 0) ? 0 : 100 - finishedPerc;

	outfile << "4- Percentage of finished orders: " << finishedPerc << "%\n\n";
	outfile << "   Percentage of cancelled orders: " << cancelledPerc << "%\n\n";

	double overwaitPerc = (totalFinished == 0) ? 0 : (overwaitCount / totalFinished) * 100;
	outfile << "5- Percentage of overwait orders: " << overwaitPerc << "%\n\n";

	double avgTi = (totalFinished == 0) ? 0 : sumTi / totalFinished;
	double avgTc = (totalFinished == 0) ? 0 : sumTc / totalFinished;
	double avgTw = (totalFinished == 0) ? 0 : sumTw / totalFinished;
	double avgTserv = (totalFinished == 0) ? 0 : sumTserv / totalFinished;

	outfile << "6- Average Ti: " << avgTi << ", Average Tc: " << avgTc
		<< ", Average Tw: " << avgTw << ", Average Tserv: " << avgTserv << "\n\n";

	double scootersUtil = 0;
	if (TotalScooters > 0 && GetCurrentTimeStep() > 0)
	{
		scootersUtil = ( TotalScootersBusyTime / (double)(GetCurrentTimeStep() * TotalScooters)) * 100.0;
	}

	double chefsUtil = 0;
	int totalChefs = TotalCN + TotalCS;
	if (totalChefs > 0 && GetCurrentTimeStep() > 0)
	{
		chefsUtil = ( TotalChefsBusyTime / (double)(GetCurrentTimeStep() * totalChefs)) * 100.0;
	}
	outfile << "7- Scooters utilization: " << scootersUtil << "%\n\n";
	outfile << "8- Chefs utilization: " << chefsUtil << "%\n\n";

	double totalDelivery = countOVN + countOVC + countOVG + countCombo;
	double failuresPerc = (totalDelivery == 0) ? 0 : ((double)TotalScooterFailures / totalDelivery) * 100;

	outfile << "9- Scooter Failures: " << TotalScooterFailures << " (Percentage: " << failuresPerc << "%)\n\n";
	
	outfile.close();
}