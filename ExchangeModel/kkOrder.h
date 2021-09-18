#pragma once
#include <cassert>
#include "kkMessage.h"


class kkOrder
{
public:
	unsigned int id;
	unsigned long long int time;
	unsigned int agentid;
	short side; // 1 buy, -1 sell
	unsigned int shares;
	int price;
	bool IOC;
public:
	kkOrder(unsigned int id, unsigned long long int time, unsigned int agentid, short side, unsigned int shares, int price, bool IOC = false);
	kkOrder(unsigned int id, const kkMessage& msg);
	bool operator<(const kkOrder& rhs) const
	{ 
		// model price-time priority, assumes same side
		assert(side == rhs.side);
		return (side * (price - rhs.price) > 0) || ((price == rhs.price) && (time < rhs.time)); 
	}
	bool IsCrossing(const kkOrder& rhs);
	bool IsEmpty();
	friend std::ostream& operator<<(std::ostream& os, const kkOrder& ord)
	{
		os << ord.id << (ord.side == 1 ? " BUY " : " SELL ") << ord.shares << "@" << ord.price << " time=" << ord.time << " account=" << ord.agentid << (ord.IOC ? " IOC " : " GTC ") << std::endl;
		return os;
	}
};


struct kkOrderPtrPriceTimeComparator // operator <
{
	bool operator()(const kkOrder* a, const kkOrder* b) const
	{
		assert(a->side == b->side);
		return (a->side * (a->price - b->price) > 0) || ((a->price == b->price) && (a->time < b->time)) || ((a->price == b->price) && (a->time == b->time) && (a->id < b->id));
		// models price-time priority; but added the last check for order-id to ensure that equivalence would not be established between two concurrent orders with same price,
		// so that find operator in Cancel method does not lead to canceling wrong orders.
	}
};

