#pragma once

#include "kkBook.h"
#include "kkMessageDriver.h"

class kkMessageProcessor
{
	kkBook & book;
	kkMessageDriver & messagedriver;
public:
	kkMessageProcessor(kkBook& book, kkMessageDriver & messagedriver) : book(book), messagedriver(messagedriver) {}
	~kkMessageProcessor() {};
	void ProcessMessagesUpToTime(unsigned long long int currenttime)
	{
		// Process messages to exchange
		kkMessagesQueue::iterator it = messagedriver.messagesqueues[0].begin();
		bool res = false;
		while ((it != messagedriver.messagesqueues[0].end()) && (it->GetArriveTime() <= currenttime))
		{
			if (it->GetMsgType() == ADD)
			{
				res |= book.Add(*it);
			}
			else if (it->GetMsgType() == MODIFY)
			{
				res |= book.Modify(*it);
			}
			else if (it->GetMsgType() == CANCEL)
			{
				res |= book.Cancel(*it);
			}
			else
			{
				std::cout << "ERROR: Wrong message type sent to exchange! Ignoring." << std::endl;
			}
			it = messagedriver.messagesqueues[0].erase(it);
		}
		if (res) { book.CreateSnapshot(currenttime); }

		// Process messages to agents
	}
};