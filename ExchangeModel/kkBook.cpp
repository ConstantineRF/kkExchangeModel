#include "kkBook.h"

bool kkBook::Add(unsigned long long int arrivetime, unsigned int agentid, short side, unsigned int shares, int price, bool IOC, unsigned int orderid)
{
	kkOrder* orderptr = new kkOrder((orderid == 0) ? nextorderid++ : orderid, arrivetime, agentid, side, shares, price, IOC);
	messagedriver.CreateMessage(ADDACK, agentid, arrivetime, arrivetime+agents[agentid].GetLatency(), orderptr->id, orderptr->agentid, orderptr->side, orderptr->shares, orderptr->price, orderptr->IOC);

	kkBookSide* basks;
	basks = (side == 1) ? (&asks) : (&bids);
	// Crosses with opposite side of the book
	bool stillcrossing = true;
	bool messagecreated = true;
	for (kkBookSideIterator it = basks->begin(); (it != basks->end()) && (stillcrossing) && (orderptr->shares > 0); )
	{
		if (orderptr->IsCrossing(**it))
		{
			unsigned int crossshares = std::min((*it)->shares, orderptr->shares);
			int crossprice = (*it)->price;
			orderptr->shares -= crossshares;
			messagecreated = messagedriver.CreateMessage(orderptr->shares==0 ? EXECUTED : PARTEXECUTED, agentid, arrivetime, arrivetime + agents[agentid].GetLatency(), orderptr->id, orderptr->agentid, orderptr->side, crossshares, (*it)->price, orderptr->IOC);
			if (!messagecreated) { std::cout << "Failed to create execution message" << std::endl; }
			(*it)->shares -= crossshares;
			messagecreated = messagedriver.CreateMessage((*it)->shares == 0 ? EXECUTED : PARTEXECUTED, (*it)->agentid, arrivetime, arrivetime + agents[(*it)->agentid].GetLatency(), (*it)->id, (*it)->agentid, (*it)->side, crossshares, (*it)->price, (*it)->IOC);
			if (!messagecreated) { std::cout << "Failed to create execution message" << std::endl; }
			if ((*it)->shares == 0)
			{
				unsigned int rhsorderid = (*it)->id;
				delete *it;
				it = basks->erase(it);
				id2order.erase(rhsorderid);
			}
			else
			{
				it++;
			}
		}
		else
		{
			stillcrossing = false;
		}
	} // end loop scanning the book
	if (orderptr->shares == 0)
	{
		delete orderptr;
	}
	else
	{
		if (orderptr->IOC)
		{

			delete orderptr;
			messagedriver.CreateMessage(CANCELACK, agentid, arrivetime, arrivetime + agents[agentid].GetLatency(), orderptr->id, orderptr->agentid, orderptr->side, orderptr->shares, orderptr->price, orderptr->IOC);
		}
		else
		{
			if (orderptr->side == 1) { bids.insert(orderptr); }
			else { asks.insert(orderptr); }

			id2order.insert(std::pair<unsigned int, kkOrder*>(orderptr->id, orderptr));
		}
	}
	return true;
}

bool kkBook::Add(const kkMessage & msg)
{
	return Add(msg.GetArriveTime(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
}

bool kkBook::Modify(const kkMessage & msg)
{
	kkOrder* orderPtr = id2order[msg.GetOrderId()];
	
	if (orderPtr == nullptr) // Did not find the order in the book
	{
		messagedriver.CreateMessage(MODIFYFAIL, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), 0, msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
		return false;
	}
	else // order found
	{
		assert(orderPtr->id == msg.GetOrderId());
		if (orderPtr->side != msg.GetSide()) // but it is on the opposite side
		{
			std::cout << "Error: Side Mismatch for modify. Message = " << msg << std::endl;
			messagedriver.CreateMessage(MODIFYFAIL, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
			return false;
		}
		else if (orderPtr->agentid != msg.GetAgentId()) // same side, but belongs to a different agent
		{
			std::cout << "Error: " << msg.GetAgentId() << " attempts to modify order of " << orderPtr->agentid << ". Message = " << msg << std::endl;
			messagedriver.CreateMessage(MODIFYFAIL, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
			return false;
		}
		else // No problems
		{
			Cancel(orderPtr->id);
			// Invoke a version of Add method that forces (old) version of order id. This way I do not
			// need to reimplement all the crosses that might happen after modifying the algo.
			Add(msg.GetArriveTime(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC(), msg.GetOrderId());
			messagedriver.CreateMessage(MODIFYACK, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
			return true;
		}
	}
}

bool kkBook::Cancel(unsigned int id) // this variant is made private intentionally, because it does not perform certain mandatory checks - those are 
											//encoded in other Cancel or Modify methods, that leverage this one.
{
	kkOrder* orderPtr = id2order[id];
	assert(orderPtr->id == id);
	// First eliminate it from the book
	if (orderPtr->side == 1)
	{
		bids.erase(orderPtr);
	}
	else
	{
		asks.erase(orderPtr);
	}
	id2order.erase(orderPtr->id);
	delete orderPtr;
	return true;
}

bool kkBook::Cancel(const kkMessage & msg)
{
	kkOrder* orderPtr = id2order[msg.GetOrderId()];
	if (orderPtr == nullptr) // Did not find the order in the book
	{
		messagedriver.CreateMessage(CANCELFAIL, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
		return false;
	}
	else // order found
	{
		assert(orderPtr->id == msg.GetOrderId());
		if (orderPtr->side != msg.GetSide()) // but it is on the opposite side
		{
			std::cout << "Error: Side Mismatch. Message = " << msg << std::endl;
			messagedriver.CreateMessage(CANCELFAIL, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
			return false;
		}
		else if (orderPtr->agentid != msg.GetAgentId()) // same side, but belongs to a different agent
		{
			std::cout << "Error: " << msg.GetAgentId() << " attempts to modify or cancel order of " << orderPtr->agentid << ". Message = " << msg << std::endl;
			messagedriver.CreateMessage(CANCELFAIL, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), msg.GetShares(), msg.GetPrice(), msg.GetIOC());
			return false;
		}
		else // No problems - proceed with erasing the order
		{
			messagedriver.CreateMessage(CANCELACK, msg.GetAgentId(), msg.GetArriveTime(), msg.GetArriveTime() + agents[msg.GetAgentId()].GetLatency(), msg.GetOrderId(), msg.GetAgentId(), msg.GetSide(), orderPtr->shares, orderPtr->price, orderPtr->IOC);
			return Cancel(orderPtr->id);
		}
	}
}
