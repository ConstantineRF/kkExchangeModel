#pragma once
#include <set>
#include <map>
#include <algorithm>
#include "kkMessage.h"
#include "kkOrder.h"
#include "kkMessageDriver.h"
#include "kkAgent.h"

typedef std::set<kkOrder*, kkOrderPtrPriceTimeComparator> kkBookSide;
typedef std::set<kkOrder*, kkOrderPtrPriceTimeComparator>::const_iterator kkBookSideConstIterator;
typedef std::set<kkOrder*, kkOrderPtrPriceTimeComparator>::iterator kkBookSideIterator;
//class kkMessage;

class kkBook
{
	kkBookSide bids, asks; // The primary repository of all orders
	std::map<unsigned int, kkOrder*> id2order;
	kkMessageDriver & messagedriver;
	kkAgentsMap & agents;
	unsigned int nextorderid;

public:
	kkBook(kkMessageDriver & messagedriver, kkAgentsMap & agents) : messagedriver(messagedriver), agents(agents), nextorderid(1) {}
	bool Add(unsigned long long int arrivetime, unsigned int agentid, short side, unsigned int shares, int price, bool IOC = false, unsigned int orderid = 0);
	bool Add(const kkMessage& msg);
	bool Modify(const kkMessage& msg);
private:
	bool Cancel(unsigned int id);
public:
	bool Cancel(const kkMessage& msg);

	friend std::ostream& operator<<(std::ostream& os, const kkBook& book)
	{
		os << " -- ASKS -- " << std::endl;
		for (kkBookSideConstIterator it = book.asks.cbegin(); it != book.asks.cend(); it++)
		{
			os << *(*it);
		}
		os << " -- BIDS -- " << std::endl;
		for (kkBookSideConstIterator it = book.bids.cbegin(); it != book.bids.cend(); it++)
		{
			os << *(*it);
		}
		return os;
	}

};

