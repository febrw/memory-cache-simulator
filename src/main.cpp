#include "cache-simulator.cpp"


int main(int argc, char * argv[])
{
	// Process Command Line Arguments
	if (argc != 3) {
		std::cout << "Usage:\n" << argv[0] << " <path_to_json_config> <path_to_trace_file>\n";
		return 0; 
	}

	// Begin JSON Parsing
	std::string path_json(argv[1]);
	std::ifstream f(path_json.data());
	json j = json::parse(f);

	// Create new cache simulator
	CacheSimulator sim(j);

	// Setup for file reading
	std::string path_to_trace_file(argv[2]);
	struct stat sb;
	uintmax_t counter = 0;
	int fd;
	const uintmax_t lineLength = 40;
	fd = open(path_to_trace_file.data(), O_RDONLY);
	fstat(fd, &sb);
	const char * data = static_cast<const char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0u));

	while (counter < sb.st_size)
	{		
		Trace t;
		
		t.mem_address = strtoul(data + counter + 17, nullptr, 16); // base 16
		t.byte_count = strtoul(data + counter + 36, nullptr, 10); // base 10

		// Run trace on caches
		sim.runTrace(t);
		counter += lineLength;
	}

	close(fd);
    sim.printJSON();
	return 0;
}
