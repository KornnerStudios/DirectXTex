// DirectXTexDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DirectXTexDLL.h"


// This is an example of an exported variable
DIRECTXTEXDLL_API int nDirectXTexDLL=0;

// This is an example of an exported function.
DIRECTXTEXDLL_API int fnDirectXTexDLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see DirectXTexDLL.h for the class definition
CDirectXTexDLL::CDirectXTexDLL()
{
	return;
}

extern "C"
DIRECTXTEXDLL_API UINT32
APIENTRY
DirectXTex_GetPointerSize()
{
	return sizeof(void*);
}

extern "C"
DIRECTXTEXDLL_API UINT32
APIENTRY
DirectXTex_GetLibraryMode()
{
#if _DEBUG
	return 0;
#else
	return 1;
#endif
}

void s_directx_texmetadata::FromNative(
	const DirectX::TexMetadata& native
)
{
	Width = static_cast<UINT32>(native.width);
	Height = static_cast<UINT32>(native.height);
	Depth = static_cast<UINT32>(native.depth);
	ArraySize = static_cast<UINT32>(native.arraySize);
	MipLevels = static_cast<UINT32>(native.mipLevels);
	MiscFlags = static_cast<UINT32>(native.miscFlags);
	MiscFlags2 = static_cast<UINT32>(native.miscFlags2);
	Format = static_cast<UINT32>(native.format);
	Dimension = static_cast<UINT32>(native.dimension);
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_GetMetadataFromFile(
	const e_directx_tex_file_type fileType,
	s_directx_texmetadata& metadata,
	const wchar_t* szFile,
	const DWORD flags
)
{
	if (fileType <= e_directx_tex_file_type::Unknown ||
		fileType >= e_directx_tex_file_type::Count)
		return E_INVALIDARG;
	if (szFile == nullptr)
		E_POINTER;

	auto hr = E_NOINTERFACE;
	DirectX::TexMetadata native_metadata;
	switch (fileType)
	{
	case e_directx_tex_file_type::DDS:
		hr = GetMetadataFromDDSFile(szFile, flags, native_metadata);
		break;
	case e_directx_tex_file_type::HDR:
		hr = GetMetadataFromHDRFile(szFile, native_metadata);
		break;
	case e_directx_tex_file_type::TGA:
		hr = GetMetadataFromTGAFile(szFile, native_metadata);
		break;
	case e_directx_tex_file_type::WIC:
		hr = GetMetadataFromWICFile(szFile, flags, native_metadata);
		break;
	}

	if (FAILED(hr))
		return hr;

	metadata.FromNative(native_metadata);

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_GetMetadataFromMemory(
	const e_directx_tex_file_type fileType,
	s_directx_texmetadata& metadata,
	const void* pSource,
	const UINT32 size,
	const DWORD flags
)
{
	if (fileType <= e_directx_tex_file_type::Unknown ||
		fileType >= e_directx_tex_file_type::Count)
		return E_INVALIDARG;
	if (pSource == nullptr)
		E_POINTER;

	auto hr = E_NOINTERFACE;
	DirectX::TexMetadata native_metadata;
	switch (fileType)
	{
	case e_directx_tex_file_type::DDS:
		hr = GetMetadataFromDDSMemory(pSource, size, flags, native_metadata);
		break;
	case e_directx_tex_file_type::HDR:
		hr = GetMetadataFromHDRMemory(pSource, size, native_metadata);
		break;
	case e_directx_tex_file_type::TGA:
		hr = GetMetadataFromTGAMemory(pSource, size, native_metadata);
		break;
	case e_directx_tex_file_type::WIC:
		hr = GetMetadataFromWICMemory(pSource, size, flags, native_metadata);
		break;
	}

	if (FAILED(hr))
		return hr;

	metadata.FromNative(native_metadata);

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_BlobNew(
	_Outptr_ DirectX::Blob** const outBlob
)
{
	if (outBlob == nullptr)
		return E_POINTER;

	*outBlob = nullptr;

	auto blob = new DirectX::Blob();
	*outBlob = blob;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_BlobFree(
	DirectX::Blob* blob
)
{
	if (blob != nullptr)
		delete blob;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_BlobGetBuffer(
	const DirectX::Blob* blob,
	void** const bufferPointer,
	UINT32* const bufferSize
)
{
	if (blob == nullptr || bufferPointer == nullptr || bufferSize == nullptr)
		return E_POINTER;

	*bufferPointer = nullptr;

	*bufferPointer = blob->GetBufferPointer();
	*bufferSize = static_cast<UINT32>(blob->GetBufferSize());

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageNew(
	DirectX::ScratchImage** const outImage
)
{
	if (outImage == nullptr)
		return E_POINTER;

	*outImage = nullptr;

	auto image = new DirectX::ScratchImage();
	*outImage = image;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageFree(
	DirectX::ScratchImage* image
)
{
	if (image != nullptr)
		delete image;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageGetMetadata(
	const DirectX::ScratchImage* image,
	s_directx_texmetadata& metadata
)
{
	if (image == nullptr)
		return E_POINTER;

	metadata.FromNative(image->GetMetadata());

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageGetRawBytes(
	const DirectX::ScratchImage* image,
	const UINT32 arrayIndex,
	const UINT32 mip,
	const UINT32 slice,
	DirectX::Blob** const outBlob
)
{
	if (image == nullptr || outBlob == nullptr)
		return E_POINTER;

	*outBlob = nullptr;

	auto metadata = image->GetMetadata();
	if (arrayIndex >= metadata.arraySize)
		return E_BOUNDS;
	if (mip >= metadata.mipLevels)
		return E_BOUNDS;

	auto subImage = image->GetImage(mip, arrayIndex, slice);
	if (subImage == nullptr)
		return E_UNEXPECTED;

	size_t rowSize = (subImage->width * DirectX::BitsPerPixel(subImage->format)) / k_bits_per_byte;
	size_t sizeInBytes = subImage->height * rowSize;

	DirectX::Blob* blob;
	auto hr = DirectXTex_BlobNew(&blob);
	if (FAILED(hr))
		return hr;

	hr = blob->Initialize(sizeInBytes);
	if (FAILED(hr))
	{
		DirectXTex_BlobFree(blob);
		return hr;
	}

	// copy row by row
	auto dst = reinterpret_cast<BYTE*>(blob->GetBufferPointer());
	for (size_t y = 0, dst_offset = 0; y < subImage->height; y++, dst_offset += rowSize)
	{
		const BYTE* src = subImage->pixels + (y * subImage->rowPitch);
		errno_t err = memcpy_s(&dst[dst_offset], rowSize, src, rowSize);
		UNREFERENCED_PARAMETER(err);
		assert(err == 0);
	}

	*outBlob = blob;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageGenerateMipMaps(
	const DirectX::ScratchImage* image,
	DirectX::ScratchImage** const outMipChain
)
{
	if (image == nullptr || outMipChain == nullptr)
		return E_POINTER;

	*outMipChain = nullptr;

	auto metadata = image->GetMetadata();
	if (metadata.mipLevels > 1)
		return ERROR_SUCCESS;

	if (DirectX::IsCompressed(metadata.format))
		return ERROR_NOT_SUPPORTED;

	DirectX::ScratchImage* mipChain;
	auto hr = DirectXTex_ScratchImageNew(&mipChain);
	if (FAILED(hr))
		return hr;

	hr = DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), metadata,
		DirectX::TEX_FILTER_FANT, 0, *mipChain);
	if (FAILED(hr))
	{
		DirectXTex_ScratchImageFree(mipChain);
		return hr;
	}

	*outMipChain = mipChain;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageCreateEmptyMipChain(
	const DirectX::ScratchImage* image,
	DirectX::ScratchImage** const outMipChain
)
{
	if (image == nullptr || outMipChain == nullptr)
		return E_POINTER;

	*outMipChain = nullptr;

	auto metadata = image->GetMetadata();
	if (metadata.mipLevels > 1)
		return ERROR_SUCCESS;

	if (DirectX::IsCompressed(metadata.format))
		return ERROR_NOT_SUPPORTED;

	DirectX::ScratchImage* mipChain;
	auto hr = DirectXTex_ScratchImageNew(&mipChain);
	if (FAILED(hr))
		return hr;

	if (metadata.miscFlags & DirectX::TEX_MISC_TEXTURECUBE)
	{
		hr = mipChain->InitializeCube(metadata.format, metadata.width, metadata.height, metadata.arraySize / 6, 0);
	}
	else
	{
		hr = mipChain->Initialize2D(metadata.format, metadata.width, metadata.height, metadata.arraySize, 0);
	}
	if (FAILED(hr))
	{
		DirectXTex_ScratchImageFree(mipChain);
		return hr;
	}

	for (size_t subItemIndex = 0; subItemIndex < metadata.arraySize; subItemIndex++)
	{
		// Filter is not needed since source and dest formats are the same
		const auto filter = DXGI_FORMAT_UNKNOWN;

		hr = DirectX::CopyRectangle(
			*image->GetImage(0, subItemIndex, 0),
			DirectX::Rect(0, 0, metadata.width, metadata.height),
			*mipChain->GetImage(0, subItemIndex, 0),
			filter,
			0, 0);
		if (FAILED(hr))
		{
			DirectXTex_ScratchImageFree(mipChain);
			return hr;
		}

		for (size_t mip = 1; mip < mipChain->GetMetadata().mipLevels; mip++)
		{
			auto subImage = mipChain->GetImage(mip, subItemIndex, 0);
			auto subImagePixels = subImage->pixels;
			auto subImagePixelSize = subImage->rowPitch * subImage->height;
			ZeroMemory(subImagePixels, subImagePixelSize);
		}
	}

	if (FAILED(hr))
	{
		DirectXTex_ScratchImageFree(mipChain);
		return hr;
	}

	*outMipChain = mipChain;

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageCreateTexture2D(
	BYTE* pixels,
	const UINT32 width,
	const UINT32 height,
	const UINT32 format,
	const UINT32 rowPitch,
	const UINT32 slicePitch,
	DirectX::ScratchImage** const outImage
)
{
	if (pixels == nullptr || outImage == nullptr)
		return E_POINTER;

	*outImage = nullptr;

	auto native_format = static_cast<DXGI_FORMAT>(format);
	if (!DirectX::IsValid(native_format))
		return E_INVALIDARG;

	DirectX::Image srcImage = {};
	srcImage.width = width;
	srcImage.height = height;
	srcImage.format = native_format;
	srcImage.rowPitch = rowPitch;
	srcImage.slicePitch = slicePitch;
	// this is the reason pixels parameter can't be const
	srcImage.pixels = pixels;

	DirectX::ScratchImage* scratchImage;
	auto hr = DirectXTex_ScratchImageNew(&scratchImage);
	if (FAILED(hr))
		return hr;

	hr = scratchImage->InitializeFromImage(srcImage);

	if (FAILED(hr))
	{
		DirectXTex_ScratchImageFree(scratchImage);
		return hr;
	}

	return ERROR_SUCCESS;
}

extern "C"
_Use_decl_annotations_
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageCreateTexture11(
	DirectX::ScratchImage* image,
	ID3D11Device* device,
	ID3D11Resource** const outTexture
)
{
	if (image == nullptr || device == nullptr || outTexture == nullptr)
		return E_POINTER;

	*outTexture = nullptr;

	ID3D11Resource* texture = nullptr;
	HRESULT hr = DirectX::CreateTexture(
		device,
		image->GetImages(),
		image->GetImageCount(),
		image->GetMetadata(),
		&texture);

	if (FAILED(hr))
		return hr;

	*outTexture = texture;

	return ERROR_SUCCESS;
}

