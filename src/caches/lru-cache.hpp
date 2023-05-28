#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

/* LRUCache represents a cache which uses a Least Recently Used replacement policy. */
class LRUCache : public Cache
{
public:
    // see Cache::Cache()
    LRUCache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name);
    ~LRUCache();
    bool lookup(uint64_t mem_address);

private:
    /* 
		Stores an array of recencies for LRU replacement.
		For cache line index i:
			for a tag at m_blocks[i]
			the recency for this tag is stored in m_recencies[i]
        A higher recency indicates this cache line was accessed more recently.
        To find LRU, get the cache line with lowest recency.
	*/
	uint64_t * m_recencies;

    /* 
        On every lookup, increment the global age.
        On a cache hit or miss, we will update the
        recency for this tag to store the global age.
    */
	uint64_t m_global_age;
};

#endif /* LRU_CACHE_HPP */
