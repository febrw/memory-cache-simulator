# Memory Cache Simulator

This project simulates various memory caching hierarchies and strategies on a set of large program trace files. A cache configuration  is given in a JSON format, specifying:

- The number of caches in the configuration
- The name of each cache (L1, L2, L3)
- The size of each cache (bytes)
- Size of a single cache line (bytes)
- Cache replacement policy (Least Recently Used, Least Frequently Used, Round-Robin)
- Cache type (direct mapped, fully associative, n-way set associative)

Each trace file entry has the format:

    <program counter address> <memory address> <memory operation kind> <size>
    
- program counter is not required by this simulator
- memory address is the base address of the access
- memory operation kind may be a read or a write
- size indicates the number of bytes being accessed (could extend over multiple cache lines)


This program takes a cache configurarion file and a trace file as command line inputs. The caching behaviour is simulated, before reporting output statistics (hit rate per cache, main memory accesses, total time taken) to a JSON file.


Compilation:
```bash
g++ main.cpp -O3 -o cache-simulator
```
Usage:
```bash
./cache-simulator <path_to_json_config> <path_to_trace_file>
```
