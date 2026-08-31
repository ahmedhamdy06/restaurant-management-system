#include "Ui.h"

UI::UI()
{
}

UI::MODE UI::getInterfaceMode() const
{
	return mode;
}

void UI::readInterfaceMode()
{
	cout << "Please select simulation mode\n";
	cout << "-----------------------------\n";
	cout << "[1] - Interactive mode\n";
	cout << "[2] - Silent mode\n";

	int choose;
	cin >> choose;
	while (cin.fail() || (choose < 1 || choose >2))
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Please, enter a valid choice\n";
		cin >> choose;
	}
	switch (choose)
	{
	case 1:
		mode = MODE::INTERACTIVE;
		break;
	case 2:
		mode = MODE::SILENT;
		break;
	}
}

void UI::readInputFileName(string& file) {
    cout << "Enter the input file name:\n";
    cin >> file;
}

void UI::readOutputFileName(string& file)
{
    cout << "Enter the output file name:\n";
    cin >> file;
}

void UI::PrintOutputScreen(int timestep, LinkedQueue<Action*>& Actions, LinkedQueue<Order*>& PEND_ODG,
	LinkedQueue<Order*>& PEND_ODN, LinkedQueue<Order*>& PEND_OT, LinkedQueue<Order*>& PEND_OVN,
	DerivedQueue& PEND_OVC, priQueue<Order*>& PEND_OVG, LinkedQueue<Chef*>& Free_CS,
	LinkedQueue<Chef*>& Free_CN, Cook_Ords& Cooking_Orders, LinkedQueue<Order*>& RDY_OT,
	LinkedQueue<Order*>& RDY_OD, DerivedQueue& RDY_OV, priQueue<Order*>& InServ_Orders,
	ArrayStack<Order*>& Finished_orders, LinkedQueue<Order*>& Cancelled_orders,
	priQueue<Scooter*>& Free_Scooters, priQueue<Scooter*>& Back_Scooters,
	LinkedQueue<Scooter*>& Maint_Scooters, Fit_Tables &Free_Tables,
	Fit_Tables &Busy_Sharable_Tables, Fit_Tables &Busy_No_Share_Tables, priQueue<Order*>& PEND_COMBO)
{
	cout << "Current TimeStep:" << timestep << '\n';
	cout << "=============== Actions List ===============\n";
	cout << Actions.getCount() << " actions remaining:\n";
	Actions.print(min(10,Actions.getCount()));
	cout << "\n\n";

	cout << "---------------- Pending Orders IDs ---------------\n";
	cout << PEND_ODN.getCount() << " ODN: "; PEND_ODN.print(); cout << "\n\n";
	cout << PEND_ODG.getCount() << " ODG: "; PEND_ODG.print(); cout << "\n\n";
	cout << PEND_OT.getCount() << " OT: ";  PEND_OT.print(); cout << "\n\n";
	cout << PEND_OVN.getCount() << " OVN: "; PEND_OVN.print(); cout << "\n\n";
	cout << PEND_OVC.getCount() << " OVC: "; PEND_OVC.print(); cout << "\n\n";
	cout << PEND_OVG.getCount() << " OVG: "; PEND_OVG.print(); cout << "\n\n";
	cout << PEND_COMBO.getCount() << " COMBO: "; PEND_COMBO.print(); cout << "\n\n\n";

	cout << "---------------- Available chefs IDs -----------------\n\n";
	cout << Free_CS.getCount() << " CS: "; Free_CS.print(); cout << "\n\n";
	cout << Free_CN.getCount() << " CN: "; Free_CN.print(); cout << "\n\n\n";

	cout << "----------------- Cooking orders [Orders ID, chef ID] ------------------\n\n";
	cout << Cooking_Orders.getCount() << " cooking orders: "; Cooking_Orders.print(); cout << "\n\n\n";

	cout << "----------------- Ready Orders IDs ------------------\n\n";
	cout << RDY_OD.getCount() << " OD: "; RDY_OD.print(); cout << "\n\n";
	cout << RDY_OT.getCount() << " OT: "; RDY_OT.print(); cout << "\n\n";
	cout << RDY_OV.getCount() << " OV: ";  RDY_OV.print(); cout << "\n\n\n";

	cout << "----------------- Available scooters IDs ------------------\n\n";
	cout << Free_Scooters.getCount() << " Scooters: "; Free_Scooters.print(); cout << "\n\n\n";

	cout << "----------------- Available tables [ID, capacity, free seats] ------------------\n\n";
	cout << Free_Tables.getCount() << " Tables: "; Free_Tables.print(); cout << "\n\n\n";

	cout << "----------------- In-Service orders [order ID, scooter/Table ID] -------------------\n\n";
	cout << InServ_Orders.getCount() << " Orders: "; InServ_Orders.print(); cout << "\n\n\n";

	cout << "----------------- In-maintainance scooters IDs ------------------\n\n";
	cout << Maint_Scooters.getCount() << " Scooters: "; Maint_Scooters.print(); cout << "\n\n\n";

	cout << "----------------- Scooters Back to Restaurant IDs ------------------\n\n";
	cout << Back_Scooters.getCount() << " Scooters: "; Back_Scooters.print(); cout << "\n\n\n";

	cout << "----------------- Cancelled Orders IDs ------------------\n\n";
	cout << Cancelled_orders.getCount() << " Cancelled: "; Cancelled_orders.print(); cout << "\n\n\n";

	cout << "----------------- Finished Orders IDs ------------------\n\n";
	cout << Finished_orders.getCount() << " Orders: "; Finished_orders.print(); cout << "\n\n\n";

	cout << "PRESS ENTER TO MOVE TO NEXT STEP !\n\n\n";
}
