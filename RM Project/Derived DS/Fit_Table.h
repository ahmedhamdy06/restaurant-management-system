#pragma once
#include "../Entities/Table.h"
#include "../DS/Priority Queue/PriQueue.h"

class Fit_Tables : public priQueue<Table*>
{
public:
	Table* getBest(int seats)
	{
		if (this->isEmpty()) return nullptr;
		
		priNode<Table*>* prev = nullptr;
		priNode<Table*>* curr = this->head;

		while (curr != nullptr)
		{
			double pri;

			if (curr->getItem(pri)->getAvailableSeats() >= seats)
			{
				Table* pTable = curr->getItem(pri);
				if (prev == nullptr)
				{
					this->head = curr->getNext();
				}
				else
				{
					prev->setNext(curr->getNext());
				}
				delete curr;
				this->count--;
				return pTable;
			}
			prev = curr;
			curr = curr->getNext();
		}
		return nullptr;
	}
	Table* removeTable(int id)
	{
		if (this->isEmpty()) return nullptr;
		
		double pri;
		priNode<Table*>* prev = nullptr;
		priNode<Table*>* curr = this->head;
		
		while (curr != nullptr)
		{
			if (curr->getItem(pri)->getId() == id)
			{
				Table* pTable = curr->getItem(pri);

				if (prev == nullptr)
				{
					this->head = curr->getNext();
				}
				else
				{
					prev->setNext(curr->getNext());
				}
				delete curr;
				this->count--;
				return pTable;
			}
			prev = curr;
			curr = curr->getNext();
		}

		return nullptr;
	}
};