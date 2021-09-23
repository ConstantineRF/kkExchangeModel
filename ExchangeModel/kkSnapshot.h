#pragma once
#include <vector>
#include <set>
#include "kkMessageDriver.h"


// Snapshot of market prices and basic stats
class kkSnapshot
{
public:
	unsigned long long int timestamp;
	std::vector<int>  bidprice;
	std::vector<unsigned int>  bidshares;
	std::vector<int>  askprice;
	std::vector<unsigned int>  askshares;
	kkSnapshot(unsigned long long int timestamp) : timestamp(timestamp), bidprice(5, 0), bidshares(5, 0), askprice(5, 0), askshares(5, 0) {}
	bool HasBids() const { return bidshares[0] > 0; }
	bool HasAsks() const { return askshares[0] > 0; }
	bool operator<(const kkSnapshot& rhs) const
	{
		return (timestamp < rhs.timestamp);
	}
	bool operator<(unsigned long long int rhs) const
	{
		return (timestamp < rhs);
	}
	friend std::ostream& operator<<(std::ostream& os, const kkSnapshot& ss)
	{
		os << "BOOK SNAPSHOT timestamp=" << ss.timestamp << std::endl;
		if (ss.HasAsks())
		{
			os << " -- ASKS -- " << std::endl;
			for (int level = 0; level < 5; level++)
			{
				if (ss.askshares[level] > 0)
				{
					os << level << ": " << ss.askshares[level] << "@" << ss.askprice[level] << std::endl;
				}
			}
		}
		if (ss.HasBids())
		{
			os << " -- BIDS -- " << std::endl;
			for (int level = 0; level < 5; level++)
			{
				if (ss.bidshares[level] > 0)
				{
					os << level << ": " << ss.bidshares[level] << "@" << ss.bidprice[level] << std::endl;
				}
			}
		}
		os << "BOOK SNAPSHOT END" << std::endl;
		return os;
	}
};

// should I possibly convert this to set of pointers? should I wrap this into a container? should I move snapshot read into a method of this container?
typedef std::set<kkSnapshot> kkSnapshotSeries;

struct kkSnapshotPtrTimeComparator // operator <
{
	bool operator()(const kkSnapshot* a, const kkSnapshot* b) const
	{
		assert(a->timestamp != b->timestamp);
		return (a->timestamp < b->timestamp);
	}
	/*
	bool operator()(const kkSnapshot* a, unsigned long long int b) const
	{
		assert(a->timestamp != b);
		return (a->timestamp < b);
	}
	bool operator()(unsigned long long int a, const kkSnapshot* b) const
	{
		assert(a != b->timestamp);
		return (a < b->timestamp);
	}
	*/
};


class kkSnapshotStream
{
	std::set<kkSnapshot*, kkSnapshotPtrTimeComparator> snapshotptrs;
public:
	kkSnapshotStream() {}
	~kkSnapshotStream()
	{
		for (auto it = snapshotptrs.begin(); it != snapshotptrs.end(); ++it)
		{
			delete *it;
		}
		snapshotptrs.clear();
	}
	kkSnapshot* GetSnapshotByTimestamp(unsigned long long int timestamp) const
	{
		if (snapshotptrs.empty())
		{
			return nullptr;
		}
		else if ((*snapshotptrs.begin())->timestamp > timestamp)
		{
			return nullptr;
		}
		else
		{
			auto lb = std::lower_bound(snapshotptrs.begin(), snapshotptrs.end(), &kkSnapshot(timestamp));
			if (lb != snapshotptrs.begin())
			{
				return *(--lb);
			}
			else {
				return nullptr;
			}
		}

	}
};