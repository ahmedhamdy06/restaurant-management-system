#pragma once
#include<iostream>
#include <string>
#include "../DS/Queue/LinkedQueue.h"
#include "../DS/Priority Queue/PriQueue.h"
#include "../DS/Stack/ArrayStack.h"
#include "../Derived DS/Derived Queue.h"
#include "../Derived DS/Fit_Table.h"
#include "../Derived DS/Cook_Ords.h"
#include "../Entities/Order.h"
#include "../Action/Action.h"
using namespace std;

class UI
{
public:
	enum MODE { INTERACTIVE, SILENT };
private:
	
	MODE mode;
public:
	UI();
	MODE getInterfaceMode() const;
	void readInterfaceMode();
	void readInputFileName(string &file);
	void readOutputFileName(string &file);

	void PrintOutputScreen(int timestep,
		LinkedQueue<Action*>& Actions,
		LinkedQueue<Order*>& PEND_ODG, LinkedQueue<Order*>& PEND_ODN, LinkedQueue<Order*>& PEND_OT,
		LinkedQueue<Order*>& PEND_OVN, DerivedQueue& PEND_OVC, priQueue<Order*>& PEND_OVG,
		LinkedQueue<Chef*>& Free_CS, LinkedQueue<Chef*>& Free_CN,
		Cook_Ords& Cooking_Orders,
		LinkedQueue<Order*>& RDY_OT, LinkedQueue<Order*>& RDY_OD, DerivedQueue& RDY_OV,
		priQueue<Order*>& InServ_Orders, ArrayStack<Order*>& Finished_orders,
		LinkedQueue<Order*>& Cancelled_orders, priQueue<Scooter*>& Free_Scooters,
        priQueue<Scooter*>& Back_Scooters, LinkedQueue<Scooter*>& Maint_Scooters, 
		Fit_Tables &Free_Tables, Fit_Tables &Busy_Sharable_Tables, Fit_Tables &Busy_No_Share_Tables , priQueue<Order*>& PEND_COMBO);
};