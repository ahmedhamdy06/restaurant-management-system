#pragma once
#include <iostream>
class Table {
public:
	enum TableStatus { FREE, BUSY_SHARABLE, BUSY_NO_SHARE};
	Table(int ID, int cap)
		: id(ID), capacity(cap), stat(FREE), occupiedSeats(0)
	{
	}
	int getId() const { return id; }
	int getCapacity() const { return capacity; }
	int getOccupiedSeats() const { return occupiedSeats; }
	TableStatus getStatus() const { return stat; }

	void setStatus(TableStatus s) { stat = s; }

	int getAvailableSeats() const {
		return capacity - occupiedSeats;
	}

	bool canSeat(int size) const {
		return (getAvailableSeats() >= size);
	}

	bool isFull() const {                     //helper function
		return occupiedSeats == capacity;
	}

	void occupySeats(int size, bool canShare) {
		if (canSeat(size)) {
			occupiedSeats += size;

			if (!canShare || isFull()) {
				stat = BUSY_NO_SHARE;
			}
			else
			{
				stat = BUSY_SHARABLE;
			}
		}
	}

	void releaseSeats(int size) {
		occupiedSeats = size >= occupiedSeats ? 0 : occupiedSeats - size;
		if (occupiedSeats == 0)
			stat = FREE;
		else {
			stat = BUSY_SHARABLE;
		}
	}
	void print(std::ostream& out) const {
		out << "[" << this->getId() << ", " << this->getCapacity() << ", " << this->getAvailableSeats() << "]";
	}
private:
	int id;
	int capacity;    
	int occupiedSeats;
	TableStatus stat;

};
inline std::ostream& operator<<(std::ostream& out, const Table* t) {
	if (t != nullptr) {
		t->print(out);
	}
	return out;
}