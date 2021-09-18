#include "kkOrder.h"

kkOrder::kkOrder(unsigned int id, unsigned long long int time, unsigned int agentid, short side, unsigned int shares, int price, bool IOC) :
	id(id), time(time), agentid(agentid), side(side), shares(shares), price(price), IOC(IOC) {}

kkOrder::kkOrder(unsigned int id, const kkMessage & msg) : id(id), time(msg.GetArriveTime()), agentid(msg.GetAgentId()), side(msg.GetSide()), shares(msg.GetShares()), price(msg.GetPrice()), IOC(msg.GetIOC()) {}

bool kkOrder::IsCrossing(const kkOrder & rhs)
{
	assert(side * rhs.side == -1);
	return side * (price - rhs.price) >= 0;
}

bool kkOrder::IsEmpty() { return shares == 0; }
