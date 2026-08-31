#pragma once

#include "../UI/Ui.h"
#include "../Entities/Order.h"
#include "../Entities/Chef.h"
#include "../Entities/Table.h"
#include "../Entities/Scooter.h"
#include "../Action/Action.h"
#include "../Action/RequestAction.h"
#include "../Action/CancelAction.h"

#include "../DS/Queue/LinkedQueue.h"
#include "../DS/Priority Queue/PriQueue.h"
#include "../DS/Stack/ArrayStack.h"

#include "../Derived DS/Derived Queue.h"
#include "../Derived DS/Cook_Ords.h"
#include "../Derived DS/Fit_Table.h"

class Restaurant
{
private:
    int currentTimeStep;
    int TH;
    int TotalCN;
    int TotalCS;
    int TotalScooters;
    int TotalChefsBusyTime;
    int TotalScootersBusyTime;
    int TotalScooterFailures;
    int MaxTableCapacity;
    UI* pUi;
    // Actions
    LinkedQueue<Action*> Actions;

    // Pending Orders
    LinkedQueue<Order*> PEND_ODG;
    LinkedQueue<Order*> PEND_ODN;
    LinkedQueue<Order*> PEND_OT;
    LinkedQueue<Order*> PEND_OVN;
    DerivedQueue PEND_OVC;
    priQueue<Order*> PEND_OVG;
    priQueue<Order*> PEND_COMBO; //combo
    // Chefs
    LinkedQueue<Chef*> Free_CS;
    LinkedQueue<Chef*> Free_CN;

    // Cooking
    Cook_Ords Cooking_Orders;

    // Ready
    LinkedQueue<Order*> RDY_OT;
    LinkedQueue<Order*> RDY_OD;
    DerivedQueue RDY_OV;

    // In Service
    priQueue<Order*> InServ_Orders;
   
    // Finished / Cancelled
    ArrayStack<Order*> Finished_orders;
    LinkedQueue<Order*> Cancelled_orders;

    // Scooters
    priQueue<Scooter*> Free_Scooters;
    priQueue<Scooter*> Back_Scooters;
    LinkedQueue<Scooter*> Maint_Scooters;

    // Tables
    Fit_Tables Free_Tables;
    Fit_Tables Busy_Sharable_Tables;
    Fit_Tables Busy_No_Share_Tables;

    priQueue<Order*> Overwait_OVG;      //Bonus
    priQueue<Order*> Rescue_Orders;

    void StartCooking(Order* pOrd, Chef* pChef); //helper

public:
    Restaurant();
    ~Restaurant();

    // Core
    void IncrementTimeStep();
    int GetCurrentTimeStep() const;
    void AssignToChefs();
    void MoveToReady();
    void FinalizeTakeaway();

    // add/cancel
    void AddOrder(Order* o);
    void CancelOrder(int id);

    int GetPendingODGCount() const;
    int GetPendingODNCount() const;
    int GetPendingOTCount() const;
    int GetPendingOVNCount() const;
    int GetPendingOVCCount() const;
    int GetPendingOVGCount() const;

    int GetFreeCNCount() const;
    int GetFreeCSCount() const;

    int GetCookingCount() const;
    int GetReadyOTCount() const;
    int GetReadyOVCount() const;
    int GetReadyODCount() const;

    int GetInServiceCount() const;
    int GetFinishedCount() const;
    int GetCancelledCount() const;

    int GetFreeScootersCount() const;
    int GetBackScootersCount() const;
    int GetMaintScootersCount() const;

    int GetFreeTablesCount() const;
    int GetBusySharableTablesCount() const;
    int GetBusyNoShareTablesCount() const;

    int GetActionCount() const;

    void ReadInputFile(string fileName);
    void GenerateOutputFile(string fileName);

    bool AssignDineIn(Order* o);
    void AssignToTables();

    void AssignToScooters();
    bool AssignScooter(Order* o);

    void CheckInServiceOrders();
    void CheckScooters();

    void CheckOverwaitOVG();
    void CheckScooterFailures();
    bool AssignRescueScooter(Order* o);

    void ExecuteActions(int currentStep);
    void RunSimulation();
};