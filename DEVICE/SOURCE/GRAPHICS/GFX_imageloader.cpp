#include "GFX_imageloader.h"

#include <wrl/client.h> // Include for ComPtr
#include <algorithm>

using namespace Microsoft::WRL;

const std::vector<ImageLoader::GUID_to_DXGI> ImageLoader::s_lookupTable =
{
	{ GUID_WICPixelFormat32bppBGRA , DXGI_FORMAT_B8G8R8A8_UNORM },
	{ GUID_WICPixelFormat32bppRGBA , DXGI_FORMAT_R8G8B8A8_UNORM },
	{ GUID_WICPixelFormat24bppBGR,   DXGI_FORMAT_B8G8R8X8_UNORM }
};

void ImageLoader::LoadImageFromDisk(const std::wstring& filePath, ImageData& data)
{
	// FACTORY

	// Allows to create instances of wic objects, like DXGI factory.
	ComPtr<IWICImagingFactory> wicFactory;

	// Initialize for WIC functionality
	HRESULT hr;
	WIC_THROW(
		CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wicFactory.GetAddressOf()))
	);
	if (FAILED(hr))
		throw DEVICE_Exception(__LINE__, __FILE__);

	// LOAD THE IMAGE
	
	// Create wic stream
	ComPtr<IWICStream> wicFileStream;
	WIC_THROW(
		wicFactory->CreateStream(wicFileStream.GetAddressOf())
	);

	// Initialize stream
	WIC_THROW(
		wicFileStream->InitializeFromFilename(filePath.c_str(), GENERIC_READ)
	);

	// Create bitmap decoder from stream.
	ComPtr<IWICBitmapDecoder> wicDecoder;
	WIC_THROW(
		wicFactory->CreateDecoderFromStream(wicFileStream.Get(), nullptr, WICDecodeMetadataCacheOnDemand, wicDecoder.GetAddressOf())
	);

	// Get frame from the bitmap (could be a gif).
	// This holds the data of the image we want to decode.
	ComPtr<IWICBitmapFrameDecode> wicFrameDecoder;
	WIC_THROW(
		wicDecoder->GetFrame(0, wicFrameDecoder.GetAddressOf())
	);

	// GET IMAGE META DATA
	WIC_THROW(
		wicFrameDecoder->GetSize(&data.width, &data.height)
	);

	WIC_THROW(
		wicFrameDecoder->GetPixelFormat(&data.wicPixelFormat)
	);

	// Get meta data of pixel format.
	ComPtr<IWICComponentInfo> wicCompInfo;
	WIC_THROW(
		wicFactory->CreateComponentInfo(data.wicPixelFormat, wicCompInfo.GetAddressOf())
	);

	ComPtr<IWICPixelFormatInfo> wicPixelFormatInfo;
	WIC_THROW(
		wicCompInfo->QueryInterface(wicPixelFormatInfo.GetAddressOf())
	);
	WIC_THROW(
		wicPixelFormatInfo->GetBitsPerPixel(&data.bpp)
	);
	WIC_THROW(
		wicPixelFormatInfo->GetChannelCount(&data.cc)
	);

	// DXGI Pixel Format

	auto findIt = std::find_if(s_lookupTable.begin(), s_lookupTable.end(), 
		[&](const GUID_to_DXGI& entry) 
		{
			return memcmp(&entry.wic, &data.wicPixelFormat, sizeof(GUID)) == 0;
		}
	);

	if (findIt == s_lookupTable.end())
	{
		throw DEVICE_Exception(__LINE__, __FILE__);
	}

	data.giPixelFormat = findIt->gi;

	// DECODE THE IMAGE
	uint32_t stride = ((data.bpp  + 7) / 8) * data.width; 
	uint32_t size = stride * data.height;
	data.data.resize(size);

	WICRect copyRect;
	copyRect.X = copyRect.Y = 0;
	copyRect.Width = data.width;
	copyRect.Height = data.height;

	WIC_THROW(
		wicFrameDecoder->CopyPixels(&copyRect, stride, size, (BYTE*)data.data.data())
	);

}
                          