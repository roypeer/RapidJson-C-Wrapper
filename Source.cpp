// Schema Validator example

// The example validates JSON text from stdin with a JSON schema specified in the argument.
#include "SchemaValidation.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <ratio>

using namespace rapidjson;

/* Constants */
static const std::string TIMER_FLAG = "-t";

/* typedefs */
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<Clock> TimePoint;

int main(int argc, char* argv[]) {
	if (argc != 3 && argc != 4) {
		std::cout << "Usage: ValidateJsonDirectory PATH_TO_SCHEMA PATH_TO_JSON_DIR (-t)" << std::endl << std::endl;
		std::cout << "PATH_TO_SCHEMA:   Path to your schema" << std::endl << 
					 "PATH_TO_JSON_DIR: Path to a directory with JSON files" << std::endl <<
			         "(optional) -t:    Time the program" << std::endl;
		return EXIT_FAILURE;
	}

	bool is_timed = ((argc == 4) && (argv[3] == TIMER_FLAG));
	TimePoint start_time, end_time;

	// Check if timer flag was provided
	if (is_timed)
	{
		// Start the timer
		start_time = Clock::now();
	}

	if (!validate_json_dir_with_schema(argv[1], argv[2]))
	{
		std::cout << "***Failed to validate JSONs***" << std::endl;
		return EXIT_FAILURE;
	}

	if(is_timed)
	{
		// Stop timer
		end_time = Clock::now();
		auto fp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
		std::cout << std::endl <<  "### Timer info ###" << std::endl;
		std::cout << "Validation Time in nanoseconds (1/10^-9 sec): " << fp_ns.count() << "ns" <<  std::endl;
		
	}
	
	
	
}
