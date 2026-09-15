#include "SYS_DEVICE_exception.h"
#include <sstream>

DEVICE_Exception::DEVICE_Exception(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* DEVICE_Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* DEVICE_Exception::GetType() const noexcept
{
	return "DEVICE_Exception";
}

int DEVICE_Exception::GetLine() const noexcept
{
	return line;
}

const std::string& DEVICE_Exception::GetFile() const noexcept
{
	return file;
}

std::string DEVICE_Exception::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[FILE] " << file << std::endl
		<< "[LINE] " << line;
	return oss.str();
}