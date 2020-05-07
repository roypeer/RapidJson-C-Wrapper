#pragma once

/* Abstract */
/*
This module will export a function to find a directory size
*/

/* Includes */
#include <string>
#include "FileExceptions.h"

/* Functions */
//************************************
// Method:    get_directory_size
// Details:   Adds up the sizes of all files in directory (non-recursive)
// Access:    public 
// Returns:   size_t - Directory size in bytes
// Parameter: const std::string directory_path
//************************************
size_t get_directory_size(const std::string directory_path);

//************************************
// Method:    get_file_size
// Details:   Finds a file size by comparing the file's end and beginning.
// Access:    public static 
// Returns:   size_t - File size in bytes
// Parameter: const std::string file_path
//************************************
static size_t get_file_size(const std::string file_path) throw(FileOpenException);


