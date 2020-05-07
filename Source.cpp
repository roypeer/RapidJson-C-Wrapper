// Schema Validator example

// The example validates JSON text from stdin with a JSON schema specified in the argument.

#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "SchemaValidation.h"
#include "DirectorySize.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <ratio>

using namespace rapidjson;

/* Constants */
static const std::string INFINITE_TIMER_FLAG = "-t";
static const double NS_TO_MS = 1000000.0;
static const double NS_TO_S = 1000000000.0;
static const double BYTES_TO_MB = 1000000.0;

/* typedefs */
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<Clock> TimePoint;


int main(int argc, char* argv[]) {
	if (argc != 3 && argc != 4) {
		std::cout << "Usage: ValidateJsonDirectory PATH_TO_SCHEMA PATH_TO_JSON_DIR (-t)" << std::endl << std::endl;
		std::cout << "PATH_TO_SCHEMA:   Path to your schema" << std::endl << 
					 "PATH_TO_JSON_DIR: Path to a directory with JSON files" << std::endl <<
			         "(optional) -t:    Time the program's validation rate (Infinite mode)" << std::endl;
		return EXIT_FAILURE;
	}

	bool is_timed = ((argc == 4) && (argv[3] == INFINITE_TIMER_FLAG));
	TimePoint start_time, end_time;

	// Check if timer flag was provided
	if (is_timed)
	{
		// Calculate total size of JSONs
		double directory_size_mb = get_directory_size(argv[2]) / BYTES_TO_MB;

		while (true) 
		{
			// Start timer
			start_time = Clock::now();

			// Validate Json
			if (!validate_json_dir_with_schema(argv[1], argv[2]))
			{
				std::cout << "***Failed to validate JSONs***" << std::endl;
				return EXIT_FAILURE;
			}

			// Stop timer
			end_time = Clock::now();

			// Retrieve duration w/ nanoseconds precision
			auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
			
			// Convert duration to seconds
			double duration_sec = duration_ns.count() / NS_TO_S;

			// Calculate validation rate and print to screen
			double validation_rate_mbs = directory_size_mb / duration_sec;
			
			system("clear"); 
			std::cout << "Validation rate: " << validation_rate_mbs << "(mb/s)" << std::endl;
		}
	}
	else 
	{
		if (!validate_json_dir_with_schema(argv[1], argv[2]))
		{
			std::cout << "***Failed to validate JSONs***" << std::endl;
			return EXIT_FAILURE;
		}
	}
}