#pragma once
/* Abstract */
/*
This module shall include:
1) a function for validating a schema against a list of jsons
2) A function to time the function (1)
*/

/* Includes */
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "JsonExceptions.h"

/* Functions*/
//************************************
// Method:    validate_json_list_with_schema
// Details:	  A function for validating a schema against a list of jsons; It wraps RapidJson's C code.
// Returns:   bool - true if validation occurred w/o exceptions
// Parameter: const std::string schema_path - path to schema
// Parameter: const std::string json_dir_path - path to directory of jsons
//************************************
bool validate_json_dir_with_schema(const std::string schema_path, const std::string json_dir_path) noexcept;


//************************************
// Method:    read_schema_to_document
// Returns:   bool - operation success
// Qualifier: noexcept
// Parameter: const std::string schema_path 
// Parameter: rapidjson::Document & document
//************************************
static bool read_schema_to_document(const std::string& schema_path, rapidjson::Document& document) noexcept;


//************************************
// Method:    validate_json_with_schema_validator
// Returns:   bool - true if JSON valid
// Qualifier: throw() - throws if there was a problem validating the JSON (Json is invalid etc.)
// Parameter: const std::string json_path - path to json
// Parameter: rapidjson::SchemaValidator & validator - validator object
// Parameter: rapidjson::Reader & reader - reader object
//************************************
static bool validate_json_with_schema_validator(const std::string json_path,
	rapidjson::SchemaValidator& validator, rapidjson::Reader& reader) throw(FileNotFoundException, InvalidJsonException);



