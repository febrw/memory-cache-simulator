#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

/* LFUCache represents a cache which uses a Least Frequently Used replacement policy. */
class LFUCache : public Cache
{
public:
	// see Cache::Cache()
	LFUCache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name);
	~LFUCache();
	bool lookup(uint64_t mem_address);
	
private:
	/* 
		Stores an array of frequencies for LFU replacement.
		For cache line index i:
			for a tag at m_blocks[i]
			the frequency for this tag is stored in m_frequencies[i]
	*/
	uint64_t * m_frequencies;
};
#endif /* LFU_CACHE_HPP */
