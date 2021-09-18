#include <iostream>
#include "kkAgent.h"
#include "kkMessage.h"
#include "kkOrder.h"
#include "kkBook.h"
#include "kkMessageDriver.h"
#include "kkMessageProcessor.h"

/*
Standard messages:
kkMessage(submittime, arrivetime, orderid, agentid, side, shares, price, IOC=false)
*/

int main()
{
	kkAgentsMap agents;
	agents.insert(kkAgentPair(1, kkAgent(1, 900)));
	agents.insert(kkAgentPair(2, kkAgent(2, 1200)));
	agents.insert(kkAgentPair(3, kkAgent(3, 1500)));
	kkMessageDriver messagedriver(3);
	//kkOrder order1(1, 87456, 101, 1, 100, 3998, false);
	//std::cout << order1 << " Everything is OK" << std::endl;
	kkBook book(messagedriver, agents);
	kkMessageProcessor messageprocessor(book, messagedriver);

	for (int currenttime = 1; currenttime < 1000000; currenttime++)
	{
		if (currenttime == 1) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[1].GetLatency(), 0, 1, 1, 100, 3999, false); }
		if (currenttime == 3) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[1].GetLatency(), 0, 1, 1, 100, 3998, false); }
		if (currenttime == 7) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[1].GetLatency(), 0, 1, 1, 100, 3997, false); }
		if (currenttime == 11) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[1].GetLatency(), 0, 1, 1, 100, 3999, false); }
		if (currenttime == 15) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[1].GetLatency(), 0, 1, 1, 100, 3998, false); }
		if (currenttime == 22) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[2].GetLatency(), 0, 2, -1, 100, 4001, false); }
		if (currenttime == 24) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[2].GetLatency(), 0, 2, -1, 100, 4002, false); }
		if (currenttime == 28) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[2].GetLatency(), 0, 2, -1, 100, 4003, false); }
		if (currenttime == 31) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[2].GetLatency(), 0, 2, -1, 100, 4001, false); }
		if (currenttime == 33) { messagedriver.CreateMessage(ADD, 0, currenttime, currenttime + agents[2].GetLatency(), 0, 2, -1, 250, 3999, false); }
		messageprocessor.ProcessMessagesUpToTime(currenttime);
	}
	/*
	book.Add(12340, 1, 1, 100, 3999);
	book.Add(12342, 1, 1, 100, 3998);
	book.Add(12344, 1, 1, 100, 3997);
	book.Add(12346, 1, 1, 100, 3999);
	book.Add(12348, 1, 1, 100, 3998);
	book.Add(12340, 2, -1, 100, 4001);
	book.Add(12342, 2, -1, 100, 4002);
	book.Add(12344, 2, -1, 100, 4003);
	book.Add(12346, 2, -1, 100, 4001);
	book.Add(12348, 2, -1, 250, 3999);
	*/
	std::cout << book;
	std::cout << messagedriver;
}