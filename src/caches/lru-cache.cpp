#include "lru-cache.hpp"

LRUCache::LRUCache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name):
    Cache(size, line_size, associativity, name) // Call the inherited Cache constructor
    {
        // and additionally initialise the global age recencies for LRU replacement
        m_global_age = 0;
        m_recencies = static_cast<uint64_t*>(calloc(size / line_size, sizeof(uint64_t)));
        if(!m_recencies)
        {
            std::cout << "Tag recencies construction failed, exiting.\n" ;
            exit(1);
        }
    }

LRUCache::~LRUCache()
{
    free(m_recencies);
    free(m_blocks);
}

bool LRUCache::lookup(uint64_t mem_address)
{
    ++m_global_age; // Update irresepective of hit or miss

    // Multiply set Index by associativity to get the cache line index of block 0 for the given set
    uint32_t index = getSetIndex(mem_address) * m_associativity;
    uint64_t tag = getTag(mem_address);

    for (int i = index; i < index + m_associativity; ++i) // HIT
    {
        if (tag == m_blocks[i])
        {
            ++m_hits;
            m_recencies[i] = m_global_age;
            return true;
        }
    }
    // We reach here if we MISSED only
    ++m_misses;
    // Get the index of the LRU element (min index if multiple min recencies)
    uint32_t lru_index = (std::min_element(m_recencies + index, m_recencies + index + m_associativity))
                            - m_recencies;

    m_blocks[lru_index] = tag; // Update at this index to store the new tag
    m_recencies[lru_index] = m_global_age; // Update the recency of the new tag to be the global age
    return false;
}
