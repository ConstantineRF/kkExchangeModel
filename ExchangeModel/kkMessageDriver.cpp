#include "kkMessageDriver.h"
kkMessageDriver::kkMessageDriver() : currentmessageid(0) { messagesqueues[0] = kkMessagesQueue(); }

kkMessageDriver::kkMessageDriver(int nagents)  : currentmessageid(0)
{
	for (int a = 0; a <= nagents; a++)
		messagesqueues[a] = kkMessagesQueue();
}

kkMessageDriver::~kkMessageDriver()
{
	std::cout << "Destroying Message Driver" << std::endl;
	for (kkMapToMessagesQueues::iterator it = messagesqueues.begin(); it != messagesqueues.end(); ++it)
	{
		std::cout << "Destroying " << (it->second.size()) << " messages for agent ID " << (it->first) << std::endl;
		it->second.clear();
		//messagesqueues.erase(it->first);
	}
	messagesqueues.clear();
}

void kkMessageDriver::RegisterNewAgent(unsigned int agentid)
{
	messagesqueues[agentid] = kkMessagesQueue();
}

bool kkMessageDriver::CreateMessage(kkMsgType msgtype, unsigned int destinationagentid, unsigned long long int submittime, unsigned long long int arrivetime, unsigned int orderid, unsigned int agentid, short side, unsigned int shares, int price, bool IOC)
{
	kkMapToMessagesQueues::iterator it = messagesqueues.find(destinationagentid);
	if (it == messagesqueues.end())
	{
		return false;
	}
	else
	{
		it->second.insert(kkMessage(currentmessageid++, msgtype, submittime, arrivetime, orderid, agentid, side, shares, price, IOC));
		return true;
	}
}


