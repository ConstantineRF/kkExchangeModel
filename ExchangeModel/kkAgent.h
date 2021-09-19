#pragma once
#include <iostream>
#include <map>
#include <algorithm>
#include "kkSnapshot.h"

class kkAgent
{
	unsigned int id;
	unsigned int latency;
	kkSnapshotSeries* snapshotsptr; // sbuscribed to prices
public:
	kkAgent() {};
	kkAgent(unsigned int id, unsigned int latency, kkSnapshotSeries* snapshotsptr) : id(id), latency(latency), snapshotsptr(snapshotsptr) { std::cout << "Created agent " << id << " with latency " << latency << std::endl; }
	unsigned int GetId() const { return id; }
	unsigned int GetLatency() const { return latency; }
	bool operator<(const kkAgent& rhs) const
	{
		// model price-time priority, assumes same side
		return id < rhs.id;
	}
	kkSnapshotSeries::iterator GetLatestSnapshot(unsigned long long int currenttime) const
	{
		if (snapshotsptr->empty())
		{
			return snapshotsptr->end();
		}
		else
		{
			auto lb = std::lower_bound(snapshotsptr->begin(), snapshotsptr->end(), kkSnapshot(std::max((unsigned long long int) 0, currenttime - GetLatency() + 1)));
			if (lb != snapshotsptr->begin())
			{
				return --lb;
			}
			else {
				return snapshotsptr->end();
			}
		}
	}
};

typedef std::map<int, kkAgent> kkAgentsMap;
typedef std::pair<int, kkAgent> kkAgentPair;