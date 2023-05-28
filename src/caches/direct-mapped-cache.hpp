#ifndef DIRECT_MAPPED_CACHE_HPP
#define DIRECT_MAPPED_CACHE_HPP

/* DirectMappedCache represents a cache which replaces a block on a miss using its index only. */
class DirectMappedCache : public Cache
{
public:
	DirectMappedCache(uint32_t size, uint32_t line_size, std::string name);
	~DirectMappedCache();
	bool lookup(uint64_t mem_address);
};
#endif /* DIRECT_MAPPED_CACHE_HPP */