#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "SchemaValidation.h"
#include <dirent.h>
#include <iostream>


using namespace rapidjson;

/* Constants */
static const size_t MAX_FILE_BUFFER_SIZE = 8192;
static const size_t ERROR_BUFFER_SIZE    = 512;
static const bool FUNC_FAIL              = false;
static const bool FUNC_SUCCESS           = true;
static const char CURR_DIR[2]			 = ".";
static const char FARHER_DIR[3]			 = "..";

bool validate_json_dir_with_schema(const std::string schema_path, const std::string json_dir_path) noexcept
{

	// Read a JSON schema from file into Document
	Document d;

	if(!read_schema_to_document(schema_path, d)) 
	{
		std::cout << "***Failed to read schema***" << std::endl;
		return FUNC_FAIL;
	}

	// Convert the Document into SchemaDocument
	SchemaDocument sd(d);

	// Initialize validator and reader
	SchemaValidator validator(sd);
	Reader reader;

	// Iterate over files in directory
	DIR* dp;
	struct dirent* ep;
	dp = opendir(json_dir_path.c_str());
	if (dp != NULL)
	{
		std::cout << "---VALIDATING JSONS---" << std::endl;
		while ((ep = readdir(dp)))  
		{
			// Check file isn't . or ..
			if (strncmp(ep->d_name, CURR_DIR, sizeof(CURR_DIR)) &&
				strncmp(ep->d_name, FARHER_DIR, sizeof(FARHER_DIR)))
			{
				try
				{
					// Use the validator and reader to validate the JSON
					std::string full_json_path = json_dir_path + "\\" + ep->d_name;
					if (validate_json_with_schema_validator(full_json_path, validator, reader))
					{
						std::cout << "JSON matches schema: " << ep->d_name << std::endl;
					}

					else
					{
						std::cout << "JSON does not match schema: " << ep->d_name << std::endl;
					}
				}

				catch (FileNotFoundException) {
					std::cout << "Couldn't find file: " << json_dir_path + "\\" + ep->d_name << std::endl;
				}
				catch (InvalidJsonException) {
					std::cout << "File is not a valid JSON format: " << ep->d_name << std::endl;
				}
				catch (std::exception& e) {
					std::cout << e.what() << std::endl;
				}
			}
		}

		std::cout << "---FINISHED VALIDATING JSONS---" << std::endl;

		
		(void)closedir(dp);
	}
	else
	{
		std::cout << "Couldn't open the directory - " << json_dir_path << std::endl;
		return FUNC_FAIL;
	}
		

	return FUNC_SUCCESS;
}

bool read_schema_to_document(const std::string& schema_path, Document& document) noexcept
{
	char schema_buffer[MAX_FILE_BUFFER_SIZE];

	// Open schema file
	FILE* fp = fopen(schema_path.c_str(), "r");
	if (!fp) {
		printf("(RapidJson) Schema file '%s' not found\n", schema_path.c_str());
		return FUNC_FAIL;
	}

	// Create a file stream with schema file & buffer
	FileReadStream fs(fp, schema_buffer, sizeof(schema_buffer));

	// Parse the stream to document object
	document.ParseStream(fs);

	// Check for parsing errors
	if (document.HasParseError()) {
		fprintf(stderr, "(RapidJson) Schema file '%s' is not a valid JSON\n", schema_path.c_str());
		fprintf(stderr, "(RapidJson) Error(offset %u): %s\n",
			static_cast<unsigned>(document.GetErrorOffset()),
			GetParseError_En(document.GetParseError()));
		fclose(fp);
		return FUNC_FAIL;
	}

	fclose(fp);
	return FUNC_SUCCESS;
}

bool validate_json_with_schema_validator(const std::string json_path, 
	rapidjson::SchemaValidator& validator, rapidjson::Reader& reader) throw(FileNotFoundException, InvalidJsonException)
{
	char json_buffer[MAX_FILE_BUFFER_SIZE];

	// Open JSON file
	FILE* json_file_pointer = fopen(json_path.c_str(), "r");
	if (!json_file_pointer) {
		std::string error_msg = "Json file not found: ";
		error_msg += json_path;
		throw(FileNotFoundException(error_msg));
	}

	// Initiate Stream 
	FileReadStream json_file_stream(json_file_pointer, json_buffer, sizeof(json_buffer));

	// Parse the JSON (using the validator) 
	if (!reader.Parse(json_file_stream, validator) && reader.GetParseErrorCode() != kParseErrorTermination) {

		// Handle parse failure (throw exception)
		char error_buffer[ERROR_BUFFER_SIZE];	
		snprintf(error_buffer, sizeof(error_buffer), "Input is not a valid JSON\n, Error(offset %u): %s\n", static_cast<unsigned>(reader.GetErrorOffset()),
			GetParseError_En(reader.GetParseErrorCode());
		throw(InvalidJsonException(error_buffer));
	}

	// Check the validation result
	if (validator.IsValid()) {
		return FUNC_SUCCESS;
	}

	return FUNC_FAIL;	
}
