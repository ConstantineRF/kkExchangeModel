#include "kkMessage.h"

kkMessage::kkMessage(unsigned long long int id, kkMsgType msgtype, unsigned long long int submittime, unsigned long long int arrivetime, unsigned int orderid, unsigned int agentid, short side, unsigned int shares, int price, bool IOC) :
	id(id), msgtype(msgtype), submittime(submittime), arrivetime(arrivetime), orderid(orderid), agentid(agentid), side(side), shares(shares), price(price), IOC(IOC) {}

kkMsgType kkMessage::GetMsgType() const { return msgtype; }

unsigned long long int kkMessage::GetArriveTime() const { return arrivetime; }

unsigned int kkMessage::GetOrderId() const { return orderid; }

unsigned int kkMessage::GetAgentId() const { return agentid; }

short kkMessage::GetSide() const { return side; }

// 1 buy, -1 sell

unsigned int kkMessage::GetShares() const { return shares; }

int kkMessage::GetPrice() const { return price; }

bool kkMessage::GetIOC() const { return IOC; }

