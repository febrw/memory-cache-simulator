#include "lfu-cache.hpp"

LFUCache::LFUCache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name):
Cache(size, line_size, associativity, name) // Call the inherited Cache constructor
{
    // and additionally initialise the frequencies for LFU replacement
    m_frequencies = static_cast<uint64_t*>(calloc(size / line_size, sizeof(uint64_t)));
    if(!m_frequencies)
    {
        std::cout << "Tag frequencies construction failed, exiting.\n" ;
        exit(1);
    }
}

LFUCache::~LFUCache()
{
    free(m_frequencies);
    free(m_blocks);
}

bool LFUCache::lookup(uint64_t mem_address)
{
    // Multiply set Index by associativity to get the cache line index of block 0 for the given set
    uint32_t index = getSetIndex(mem_address) * m_associativity;
	uint64_t tag = getTag(mem_address);

    for (int i = index; i < index + m_associativity; ++i)
    {
        if (tag == m_blocks[i]) // HIT
        {
            ++m_hits;
            ++m_frequencies[i];
            return true;
        }
    }
    // We reach here if we MISSED only
    ++m_misses;
    // Get the index of the LFU element (min index if multiple min frequency)
    uint32_t lfu_index = (std::min_element(m_frequencies + index, m_frequencies + index + m_associativity))
                            - m_frequencies;
    
    m_blocks[lfu_index] = tag; // Update at this index to store the new tag
    m_frequencies[lfu_index] = 1; // Update the frequency of the new tag to be 1
    return false;
}
