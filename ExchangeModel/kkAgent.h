#pragma once
#include <iostream>
#include <map>

class kkAgent
{
	unsigned int id;
	unsigned int latency;
public:
	kkAgent() {}
	kkAgent(unsigned int id, unsigned int latency) : id(id), latency(latency) { std::cout << "Created agent " << id << " with latency " << latency << std::endl; }
	unsigned int GetId() const { return id; }
	unsigned int GetLatency() const { return latency; }
	bool operator<(const kkAgent& rhs) const
	{
		// model price-time priority, assumes same side
		return id < rhs.id;
	}
};

typedef std::map<int, kkAgent> kkAgentsMap;
typedef std::pair<int, kkAgent> kkAgentPair;