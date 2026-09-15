#pragma once

// This class is going to handle D3D exceptions when using a device/device context.

#include "SYS_WinFilter.h" // Include for Windows.h
#include "SYS_DEVICE_exception.h"

class D3D_Exception : public DEVICE_Exception
{
public:
	D3D_Exception(int, const char*, HRESULT) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;

private:
	HRESULT hr;
	std::string info;
};

#define D3D_EXCEPT(hr) D3D_Exception( __LINE__,__FILE__,(hr))
#define D3D_THROW(hrcall) if( FAILED( hr = (hrcall) ) ) throw D3D_EXCEPT(hr)