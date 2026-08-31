#include <iostream>
#include "Restaurant/Restaurant.h"
#include <ctime>
using namespace std;

int main()
{
    srand(time(0));

    Restaurant* rest = new Restaurant();
    rest->RunSimulation();

    delete rest;
   return 0;
}