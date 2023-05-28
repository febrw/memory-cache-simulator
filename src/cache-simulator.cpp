#include <iostream>
#include <fstream>
#include <math.h>
#include <stdint.h>
#include <unordered_map>

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <queue>

#include "json.hpp"
#include "caches/cache.cpp"
#include "cache-simulator.hpp"

#include "caches/direct-mapped-cache.cpp"
#include "caches/lfu-cache.cpp"
#include "caches/lru-cache.cpp"
#include "caches/rr-cache.cpp"

#include <algorithm>
#include <cstring>

std::unordered_map<std::string, RPolicy> CacheSimulator::r_policies =
{
	{"lfu", lfu},
	{"lru", lru},
	{"rr", rr}
};

std::unordered_map<std::string, uint8_t> CacheSimulator::kinds_map =
{
	{"direct", 1},
	{"2way", 2},
	{"4way", 4},
	{"8way", 8},
	{"full", 0}
	// We will check if assoc is zero, and assign accordingly later when creaing a Full Assoc Cache
};


CacheSimulator::CacheSimulator(const json& data)
{
	num_caches = data.at("caches").size();
	caches = new Cache*[num_caches];

	for (int i = 0; i < num_caches; ++i)
	{
		caches[i] = createCache(data, i); // returns pointer to a Cache on the heap
	}
}

CacheSimulator::~CacheSimulator()
{
	delete[] caches;
}


void CacheSimulator::printJSON() const
{
    json result = json::object();
    json caches_array = json::array();
    json current_cache = json::object();

    result["main_memory_accesses"] = getMemAccesses();
    for (int i = 0; i < num_caches; ++i)
    {
        current_cache = {
            {"name", caches[i]-> getName()},
            {"hits", caches[i] -> getHits()},
            {"misses", caches[i] -> getMisses()}
        };
        caches_array.push_back(current_cache);
    }
    result["caches"] = caches_array;
    std::cout << result.dump(2);
}


void CacheSimulator::runTrace(Trace& t)
{
	uint16_t bytes_read = 0; // temp store
	uint16_t bytes_remaining = t.byte_count; // accumulator
	uint64_t mem_address = t.mem_address;
	uint64_t old_mem_add = mem_address;
	uint32_t bytes_on_this_line;

	// Loop while there are still bytes remaining for this trace
	// on each loop, decompose the trace, and feed it to the caches
	do
	{
		auto l1 = caches[0];
		bool hit = l1 -> lookup(mem_address);
		bytes_on_this_line = (l1 -> getLineSize()) - (l1 -> getOffset(mem_address));
		bytes_read = (bytes_remaining > bytes_on_this_line) ? bytes_on_this_line : bytes_remaining;
		if (!hit) {old_mem_add = mem_address;}
		mem_address += bytes_read;
		bytes_remaining -= bytes_read;

		for (int i = 1; !hit && i < num_caches; ++i)
		{
			hit = caches[i] -> lookup(old_mem_add);
		}
	}
	while (bytes_remaining != 0);
}

// creates a cache object (or derived) and returns a pointer to the base class Cache
Cache* CacheSimulator::createCache(const json& data, int i)
{
	
	auto info = data.at("caches").at(i);
	uint32_t size = info.at("size"); // get size of each cache
	uint32_t line_size = info.at("line_size"); // get size of each cache line
	std::string name = info.at("name"); // get cache's name, e.g. "L1"
	// get cache kind and replacement policy
	RPolicy replacement_policy = r_policies.at(info.value("replacement_policy", "rr"));
	uint32_t associativity = kinds_map.at(info.at("kind"));
	if (!associativity)
	{
		associativity = size / line_size;
	}
	// Cache Factory
	Cache * cache_ptr;
	if (associativity == 1)
	{
		cache_ptr = new DirectMappedCache(size, line_size, name);
	}
	else
	{
		switch(replacement_policy)
		{
			case lfu:
				cache_ptr = new LFUCache(size, line_size, associativity, name);
				break;
			case lru:
				cache_ptr = new LRUCache(size, line_size, associativity, name);
				break;
			case rr:
			default:
				cache_ptr = new RRCache(size, line_size, associativity, name);
				break;
		}
	}
	return cache_ptr;
}

uint64_t CacheSimulator::getMemAccesses() const
{
	return caches[num_caches-1] -> getMisses();
}


