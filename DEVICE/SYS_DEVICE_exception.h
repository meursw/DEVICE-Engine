#pragma once

#include <exception>
#include <string>

// Captures two kinds of data:
//		1. The line number the exception was thrown from.
//		2. The file the exception was thrown from.
// what() prints this data. 

class DEVICE_Exception : public std::exception
{
public:
	DEVICE_Exception(int line, const char* file) noexcept;
	const char* what() const noexcept override;

	// Get the type of the exception.
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;

	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	int line;
	std::string file;

protected: 
	// Mutable because what() is declared const.
	mutable std::string whatBuffer;
};

