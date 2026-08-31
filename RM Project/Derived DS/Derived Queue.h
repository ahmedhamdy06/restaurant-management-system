#pragma once
#include "../DS/Queue/LinkedQueue.h"
#include "../Entities/Order.h"

class DerivedQueue : public LinkedQueue<Order*>
{
public:
	Order* cancelOrder(int id)
	{
		if (this->isEmpty()) return nullptr;

		Node<Order*>* prev = nullptr;
		Node<Order*>* curr = this->frontPtr;

		while (curr != nullptr)
		{
			if (curr->getItem()->getId() == id)
			{
				Order* pOrd = curr->getItem();

				if (pOrd->getType() != Order::OVC)
				{
					return nullptr;
				}

				if (prev == nullptr)
				{
					this->frontPtr = curr->getNext();
					if (curr == this->backPtr)
					{
						this->backPtr = nullptr;
					}
				}
				else
				{
					if (curr == this->backPtr)
					{
						this->backPtr = prev;
					}
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

	Order* removeByType(Order::OrderType type)
	{
		if (this->isEmpty()) return nullptr;

		Node<Order*>* prev = nullptr;
		Node<Order*>* curr = this->frontPtr;

		while (curr != nullptr)
		{
			if (curr->getItem()->getType() == type)
			{
				Order* pOrd = curr->getItem();

				if (prev == nullptr)
					this->frontPtr = curr->getNext();
				else
					prev->setNext(curr->getNext());

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