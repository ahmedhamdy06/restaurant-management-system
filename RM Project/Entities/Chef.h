#pragma once
#include <iostream>
class Chef {
public:
	enum ChefStatus {AVAILABLE, BUSY};
	enum ChefType {CN, CS};
	Chef(int ID, ChefType t, int s) : id(ID), type(t), speed(s), stat(AVAILABLE)
	{
	}

	int getID() const {
		return id;
	}
	ChefType getChefType() const {
		return type;
	}
	ChefStatus getChefStat() const {
		return stat;
	}
	int getSpeed() const {
		return speed;
	}

	void setStatus(ChefStatus s) {
		stat = s;
	}

	void print(std::ostream& out) const {
		out << id;
	}

private:
	int id;
	ChefType type;
	ChefStatus stat;
	int speed;
};
inline std::ostream& operator<<(std::ostream& out, const Chef* chf) {
	if (chf != nullptr) {
		chf->print(out);
	}
	return out;
}
