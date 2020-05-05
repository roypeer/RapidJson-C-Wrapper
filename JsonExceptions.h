#pragma once
#include <exception>
#include <string>

class InvalidJsonException: public std::exception
{
	std::string _msg;
public:
	InvalidJsonException(const std::string& msg) : _msg(msg) {}

	virtual const char* what() const noexcept override
	{
		return _msg.c_str();
	}
};


class FileNotFoundException : public std::exception
{
	std::string _msg;
public:
	FileNotFoundException(const std::string& msg) : _msg(msg) {}

	virtual const char* what() const noexcept override
	{
		return _msg.c_str();
	}
};



