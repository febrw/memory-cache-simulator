#ifndef CACHE_HPP
#define CACHE_HPP

/* Stores replacement policies as enum for use in switch statements to create various caches. */
enum RPolicy {lru, lfu, rr};

/*
	Stores a base memory address, and the number of bytes to be read or written
	beginning at this base address.

	Used to decompose a multi-cache line access into its constituent Traces.
	This allows the cache simulator to decompose a long trace as required for the
	caches it controls, rather than each cache bounds checking their inputs.
*/
typedef struct {
	uint64_t mem_address;
	uint16_t byte_count;
} Trace;

/* Cache is an abstract class, whose constructor and methods are used by derived classes. */
class Cache
{	
public:
	/*
		size: 			total number of bytes in the cache
		line_size: 		the number of bytes that fit on one cache line/block
		associativity: 	the number of cache lines that any block in main memory may associate with (map to)
						e.g. 4-way assoc -> a mem block gets indexed to a set, in which the memory line could
						map to any of the 4 cache lines in that set
		name:			the name of the cache specified in the JSON configuration
	*/
	Cache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name);
	virtual ~Cache() = 0;

	/* 
	Looks up a memory address and updates hits or misses appropriately.
	Returns: 
		True if the provided memory address hits in this cache.
		False if the provided memory address misses in this cache.
	*/
	virtual bool lookup(uint64_t mem_address) = 0;

	// Getters
	std::string getName() const;
	uint64_t getHits() const;
	uint64_t getMisses() const;
	uint32_t getLineSize() const;

	// returns the set index that a memory address will map to
	uint32_t getSetIndex(uint64_t mem_address);

	// returns the byte offset within a cache line that this memory address (first byte) will map to
	uint32_t getOffset(uint64_t mem_address);

	// returns the tag from this memory address, for this cache configuration
	uint64_t getTag(uint64_t mem_address);

protected:
	uint32_t m_size, m_line_size, m_sets, m_associativity;
	uint8_t m_num_tag_bits, m_num_offset_bits, m_num_set_bits;
	uint64_t m_hits, m_misses;

	// An array storing the tags of data lines which would be in the cache
	uint64_t * m_blocks;
	std::string m_name;
};

#endif /* CACHE_HPP */