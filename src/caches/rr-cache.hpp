#ifndef RR_CACHE_HPP
#define RR_CACHE_HPP

/* RRRCache represents a cache which uses a Round Robin replacement policy. */
class RRCache : public Cache
{
public:
	// see Cache::Cache()
    RRCache(uint32_t size, uint32_t line_size, uint32_t associativity, std::string name);
    ~RRCache();
    bool lookup(uint64_t mem_address);

private:
/* 
    Stores an array of queues for Round Robin replacement.
    For set index i:
        m_queues[i] stores a queue of tags currently in set i
    The queues are initalised as full before used, so bounds checking is never performed.
*/
    std::queue<uint64_t> * m_queues;
/* 
    For each queue (corresponding to a cache set):
        fill the queue with zero tags, up to the size of the set (associativity)
        
    A set miss will not need to check if the set if full before inserting a new tag.
    Each queue will always be full, so a miss will always require a dequeue()
    before an enqueue(new tag)
*/
    void initialise_queues();

};
#endif /* RR_CACHE_HPP */
