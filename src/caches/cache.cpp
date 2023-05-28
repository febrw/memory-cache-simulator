#include "cache.hpp"
// Used by Concrete Cache types only
Cache::Cache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name):
	m_size(size),
	m_line_size(line_size),
	m_associativity(associativity), // Full cache: assoc = size / line_size, Direct-Mapped 
	m_name(std::move(name)),
	m_sets(size / (line_size * associativity))
	{
		m_num_offset_bits = static_cast<uint8_t>(log2(line_size));
		m_num_set_bits = static_cast<uint8_t>(log2(m_sets));
		m_num_tag_bits = 64 - m_num_set_bits - m_num_offset_bits;
		m_blocks = static_cast<uint64_t*>(calloc(size / line_size, sizeof(uint64_t)));
		if(!m_blocks)
		{
			std::cout << "Tag memory construction failed, exiting.\n" ;
	      	exit(1);
		}
		m_hits = 0;
		m_misses = 0;
	}

Cache::~Cache() {}

// Getters

uint64_t Cache::getTag(uint64_t mem_address)
{
	return mem_address >> (64 - m_num_tag_bits);
}

uint32_t Cache::getSetIndex(uint64_t mem_address)
{
	return mem_address << m_num_tag_bits >> (m_num_tag_bits + m_num_offset_bits);
}

// Fixed for 64-bit memory addresses
uint32_t Cache::getOffset(uint64_t mem_address)
{
	return mem_address << (64 - m_num_offset_bits) >> (64 - m_num_offset_bits);
}

std::string Cache::getName() const
{
	return m_name;
}

uint64_t Cache::getHits() const
{
	return m_hits;
}

uint64_t Cache::getMisses() const
{
	return m_misses;
}

uint32_t Cache::getLineSize() const
{
	return m_line_size;
}