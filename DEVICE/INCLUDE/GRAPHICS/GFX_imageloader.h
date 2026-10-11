#pragma once

#include <d3d11.h>
#include <vector>
#include <string>

#include "SYS_WinFilter.h"
#include "SYS_d3d_exception.h"

#include <wincodec.h>

class ImageLoader
{
public:
	struct ImageData
	{
		std::vector<char> data;
		uint32_t width;
		uint32_t height;
		// Bits per pixel.
		uint32_t bpp;
		// Channel count.
		uint32_t cc;
		
		GUID wicPixelFormat;
		DXGI_FORMAT giPixelFormat;
	};

	static void LoadImageFromDisk(const std::wstring& filePath, ImageData& data);

private:
	struct GUID_to_DXGI
	{
		GUID wic;
		DXGI_FORMAT gi;
	};
	static const std::vector<GUID_to_DXGI> s_lookupTable;

private:
	// Define this class "like" a singleton so that we dont have to instantiate it to use it.
	ImageLoader() = default;
	ImageLoader(const ImageLoader&) = default;
	ImageLoader& operator=(const ImageLoader&) = default;
};

#define D3D_EXCEPT(hr) D3D_Exception( __LINE__,__FILE__,(hr))
#define WIC_THROW(hrcall) if( FAILED( hr = (hrcall) ) ) throw D3D_EXCEPT(hr)