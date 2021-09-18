#pragma once
#include <iostream>
#include <set>
#include <map>
#include <string>


typedef enum {ADD, ADDACK, ADDFAIL, MODIFY, MODIFYACK, MODIFYFAIL, CANCEL, CANCELACK, CANCELFAIL, EXECUTED, PARTEXECUTED} kkMsgType;


//class kkBook;

class kkMessage
{
	unsigned long long int submittime, arrivetime, id;
	kkMsgType msgtype;

	unsigned int orderid;
	unsigned int agentid;
	short side; // 1 buy, -1 sell
	unsigned int shares;
	int price;
	bool IOC;
public:
	kkMessage(unsigned long long int id, kkMsgType msgtype, unsigned long long int submittime, unsigned long long int arrivetime, unsigned int orderid, unsigned int agentid, short side, unsigned int shares, int price, bool IOC = false);
	kkMsgType GetMsgType() const;
	unsigned long long int GetArriveTime() const;
	unsigned int GetOrderId() const;
	unsigned int GetAgentId() const;
	short GetSide() const; // 1 buy, -1 sell
	unsigned int GetShares() const;
	int GetPrice() const;
	bool GetIOC() const;
	bool operator<(const kkMessage& rhs) const
	{
		return (arrivetime < rhs.arrivetime) || ((arrivetime == rhs.arrivetime) && (id < rhs.id));
	}
	friend std::ostream& operator<<(std::ostream& os, const kkMessage& rhs)
	{
		if (rhs.msgtype == ADD) { os << rhs.arrivetime << " ADD " << (rhs.side == 1 ? "Buy " : "Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == ADDACK) { os << rhs.arrivetime << " ADDED " << rhs.orderid << " " << (rhs.side == 1 ? "Buy " : "Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == ADDFAIL) { os << rhs.arrivetime << " ADD Failed " << (rhs.side == 1 ? "Buy " : "Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == MODIFY) { os << rhs.arrivetime << " MODIFY " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == MODIFYACK) { os << rhs.arrivetime << " MODIFIED " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == MODIFYFAIL) { os << rhs.arrivetime << " MODIFY Failed " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == CANCEL) { os << rhs.arrivetime << " CANCEL " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == CANCELACK) { os << rhs.arrivetime << " CANCELLED " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == CANCELFAIL) { os << rhs.arrivetime << " CANCEL Failed " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == EXECUTED) { os << rhs.arrivetime << " EXECUTED " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		if (rhs.msgtype == PARTEXECUTED) { os << rhs.arrivetime << " EXECUTED-PARTIALLY " << rhs.orderid << (rhs.side == 1 ? " Buy " : " Sell ") << rhs.shares << "@" << rhs.price << (rhs.IOC ? " IOC for " : " for ") << rhs.agentid; }
		os << std::endl;
		return os;
	}
};

