#include "rr-cache.hpp"

RRCache::RRCache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name):
Cache(size, line_size, associativity, name) // Call the inherited Cache constructor
{
    // and additionally initialise the queues for RR replacement
    m_queues = new std::queue<uint64_t>[m_sets];
    initialise_queues();
}

void RRCache::initialise_queues()
{
    for (int i = 0; i < m_sets; ++i)
    {
        for (int j = 0; j < m_associativity; ++j)
        {
            m_queues[i].push(0);
        }
    }
}

RRCache::~RRCache()
{
    delete[] m_queues;
    free(m_blocks);
}

bool RRCache::lookup(uint64_t mem_address)
{
    uint32_t set_index = getSetIndex(mem_address);
    // Multiply set Index by associativity to get the cache line index of block 0 for the given set
    uint32_t start = set_index * m_associativity;
    uint64_t tag = getTag(mem_address);

    // linear scan through set to find
    for (int i = start; i < start + m_associativity; ++i)
    {   
        if (tag == m_blocks[i])
        {   
            ++m_hits;
            return true;
        }   
    }
    // We reach here if we MISSED only
    ++m_misses;
    auto & q = m_queues[set_index];
    auto evicted = q.front();
    q.pop(); // evict old cache line (dequeue)
    uint64_t evict_index = std::find(m_blocks + start, m_blocks + start + m_associativity, evicted)
                            - m_blocks;
    q.push(tag); // add new cache line (enqueue)

    m_blocks[evict_index] = tag; // Update at this index to store the new tag
    return false;
}
