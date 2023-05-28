#include "direct-mapped-cache.hpp"

DirectMappedCache::DirectMappedCache(uint32_t size, uint32_t line_size, std::string name):
	Cache(size, line_size, 1, name) {} // Call the inherited Cache constructor

DirectMappedCache::~DirectMappedCache()
{	
	free(m_blocks);
}

// True if hit, False if miss
bool DirectMappedCache::lookup(uint64_t mem_address)
{
	uint32_t index = getSetIndex(mem_address);
	uint64_t tag = getTag(mem_address);

	if (tag == m_blocks[index]) // HIT
	{
		++m_hits;
		return true;
	}
	else
	{
		++m_misses; // MISS
		m_blocks[index] = tag; // Update
		return false;
	}
}
