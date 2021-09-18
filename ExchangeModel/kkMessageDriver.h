#pragma once
#include <iostream>
#include "kkMessage.h"

// Define a class to model messages arriving to an exchange or to individual agents
typedef std::set<kkMessage> kkMessagesQueue;
typedef std::map<unsigned int, kkMessagesQueue> kkMapToMessagesQueues;
class kkMessageDriver
{
	unsigned long long int currentmessageid;
public:
	kkMapToMessagesQueues messagesqueues;
	kkMessageDriver();
	kkMessageDriver(int nagents);
	~kkMessageDriver();
	void RegisterNewAgent(unsigned int agentid);
	bool CreateMessage(kkMsgType msgtype, unsigned int destinationagentid, unsigned long long int submittime, unsigned long long int arrivetime, unsigned int orderid, unsigned int agentid, short side, unsigned int shares, int price, bool IOC = false);
	friend std::ostream& operator<<(std::ostream& os, const kkMessageDriver& d)
	{
		for (kkMapToMessagesQueues::const_iterator it = d.messagesqueues.begin(); it != d.messagesqueues.end(); ++it)
		{
			os << "Messages for agent " << (it->first) << std::endl;
			for (kkMessagesQueue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				os << (*it2);
			}
		}
		os << std::endl;
		return os;
	}

};

