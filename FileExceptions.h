#pragma once

/* Exceptions */
class FileOpenException : public std::exception
{
	std::string _file_name;
public:
	FileOpenException(const std::string& file_name) : _file_name(file_name) {}

	virtual const char* what() const noexcept override
	{
		return (_file_name + ": Could not open file").c_str();
	}
};
