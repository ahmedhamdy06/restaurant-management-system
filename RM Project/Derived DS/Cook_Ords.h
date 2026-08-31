#pragma once
#include "../Entities/Order.h"
#include "../DS/Priority Queue/PriQueue.h"

class Cook_Ords : public priQueue<Order*>
{
public:
	Order* cancelOrder(int id)
	{
		if (this->isEmpty()) return nullptr;

		priNode<Order*>* prev = nullptr;
		priNode<Order*>* curr = this->head;

		while (curr != nullptr)
		{
			double pri;
			if (curr->getItem(pri)->getId() == id) 
			{
				Order* pOrd = curr->getItem(pri);

				if (pOrd->getType() != Order::OVC)
				{
					return nullptr;
				}

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

				return pOrd;
			}
			prev = curr;
			curr = curr->getNext();
		}
		return nullptr;
	}
};