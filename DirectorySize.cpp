/* Includes */
#include <fstream>
#include <iostream>
#include <dirent.h>
#include "DirectorySize.h"

/* Constants */
static const char CURR_DIR[2] = ".";
static const char FARHER_DIR[3] = "..";
static const bool FUNC_FAIL = false;

/* Functions */

size_t get_directory_size(const std::string directory_path)
{
	size_t total_size = 0;
	DIR* dp;
	struct dirent* ep;

	// Iterate over directory. TODO: create class DirectoryIterator to prevent code duplication.
	dp = opendir(directory_path.c_str());
	if (dp != NULL)
	{
		while ((ep = readdir(dp)))
		{
			// Check file isn't . or ..
			if (strncmp(ep->d_name, CURR_DIR, sizeof(CURR_DIR)) &&
				strncmp(ep->d_name, FARHER_DIR, sizeof(FARHER_DIR)))
			{
				try
				{
					// Add file size to total size
					std::string full_file_path = directory_path + "/" + ep->d_name;
					total_size += get_file_size(full_file_path);
				}
				catch (FileOpenException)
				{
					std::cout << "(Directory size calculation) Unable to open file: " << 
						directory_path + "/" + ep->d_name << std::endl;
					return FUNC_FAIL;
				}
			}
		}

	}
	else {
		std::cout << "Couldn't open directory for size calculation." << std::endl;;
		return FUNC_FAIL;
	}

	return total_size;
}

size_t get_file_size(const std::string file_path) throw(FileOpenException)
{
	std::ifstream inspected_file(file_path, std::ios::binary);

	// Check if file failed to open
	if (!inspected_file.is_open())
	{
		throw FileOpenException(file_path);
	}
	
	// Mark file beginning
	const auto begin = inspected_file.tellg();

	// Seek to end
	inspected_file.seekg(0, std::ios::end);
	const auto end = inspected_file.tellg();

	// Compute diff
	size_t file_size = static_cast<size_t>(end - begin);

	return file_size;
}
