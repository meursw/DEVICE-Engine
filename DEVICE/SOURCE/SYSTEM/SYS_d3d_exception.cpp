#include "SYS_d3d_exception.h"

#include <sstream>

D3D_Exception::D3D_Exception(int line, const char* file, HRESULT hr) noexcept
	:
	DEVICE_Exception(line, file),
	hr(hr)
{}

const char* D3D_Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << hr << std::endl
		<< GetOriginString() << std::endl;

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* D3D_Exception::GetType() const noexcept
{
	return "HR_D3D_Exception";
}