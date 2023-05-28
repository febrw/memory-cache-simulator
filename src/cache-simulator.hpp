#ifndef CACHE_SIMULATOR_HPP
#define CACHE_SIMULATOR_HPP

/*
CacheSimulator creates caches from a JSON file specified,
and can proceess a memory trace on the caches it controls.
*/
using json = nlohmann::json;

class CacheSimulator
{
public:
	// Constructor used by derived classes
	CacheSimulator(const json& data);

	~CacheSimulator();

	// runs a memory trace of format: (memory_address, size_of_access(B)) on its caches, propagating misses as required
	void runTrace(Trace& t);

	// Gets total main memory accesses
	uint64_t getMemAccesses() const;

	// Prints the result of the simulation configuration in a JSON format
    void printJSON() const;

private:
	// Creates a new cache from a JSON object, with cache index i (zero-indexed)
	Cache* createCache(const json& data, int i);

	// Stores the number of caches in the configuration
	uint16_t num_caches;

	// Will store an array of cache pointers
	Cache ** caches;

	// For Cache creation

	// Maps replacment policy strings to enums
	static std::unordered_map<std::string, RPolicy> r_policies;

	// Maps cache kind strings to associativities
	static std::unordered_map<std::string, uint8_t> kinds_map;
};

#endif /* CACHE_SIMULATOR_HPP */
