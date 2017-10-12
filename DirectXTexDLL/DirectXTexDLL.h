// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DIRECTXTEXDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DIRECTXTEXDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DIRECTXTEXDLL_EXPORTS
#define DIRECTXTEXDLL_API __declspec(dllexport)
#else
#define DIRECTXTEXDLL_API __declspec(dllimport)
#endif

// This class is exported from the DirectXTexDLL.dll
class DIRECTXTEXDLL_API CDirectXTexDLL {
public:
	CDirectXTexDLL(void);
	// TODO: add your methods here.
};

extern DIRECTXTEXDLL_API int nDirectXTexDLL;

DIRECTXTEXDLL_API int fnDirectXTexDLL(void);

const size_t k_bits_per_byte = 8;

enum class e_directx_tex_file_type : UINT32
{
	Unknown,

	DDS,
	HDR,
	TGA,
	WIC,

	Count
};

struct s_directx_texmetadata
{
	UINT32 Width;
	UINT32 Height;
	UINT32 Depth;
	UINT32 ArraySize;
	UINT32 MipLevels;
	UINT32 MiscFlags;
	UINT32 MiscFlags2;
	UINT32 Format; // DXGI_FORMAT
	UINT32 Dimension; // DirectX::TEX_DIMENSION

	void FromNative(
		const DirectX::TexMetadata& native
	);
};

extern "C"
DIRECTXTEXDLL_API UINT32
APIENTRY
DirectXTex_GetPointerSize();
extern "C"
DIRECTXTEXDLL_API UINT32
APIENTRY
DirectXTex_GetLibraryMode();

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_GetMetadataFromFile(
	_In_ const e_directx_tex_file_type fileType,
	_Out_ s_directx_texmetadata& metadata,
	_In_z_ const wchar_t* szFile,
	_In_ const DWORD flags
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_GetMetadataFromMemory(
	_In_ const e_directx_tex_file_type fileType,
	_Out_ s_directx_texmetadata& metadata,
	_In_reads_bytes_(size) const void* pSource,
	_In_ const UINT32 size,
	_In_ const DWORD flags
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_BlobNew(
	_Outptr_ DirectX::Blob** const outBlob
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_BlobFree(
	_In_ DirectX::Blob* blob
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_BlobGetBuffer(
	_In_ const DirectX::Blob* blob,
	_Outptr_ void** const bufferPointer,
	_Outptr_ UINT32* const bufferSize
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageNew(
	_Outptr_ DirectX::ScratchImage** const outImage
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageFree(
	_In_ DirectX::ScratchImage* image
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageGetMetadata(
	_In_ const DirectX::ScratchImage* image,
	_Out_ s_directx_texmetadata& metadata
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageGetRawBytes(
	_In_ const DirectX::ScratchImage* image,
	_In_ const UINT32 arrayIndex,
	_In_ const UINT32 mip,
	_In_ const UINT32 slice,
	_Outptr_ DirectX::Blob** const outBlob
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageGenerateMipMaps(
	_In_ const DirectX::ScratchImage* image,
	_Outptr_ DirectX::ScratchImage** const outMipChain
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageCreateEmptyMipChain(
	_In_ const DirectX::ScratchImage* image,
	_Outptr_ DirectX::ScratchImage** const outMipChain
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageCreateTexture2D(
	_In_ BYTE* pixels,
	_In_ const UINT32 width,
	_In_ const UINT32 height,
	_In_ const UINT32 format,
	_In_ const UINT32 rowPitch,
	_In_ const UINT32 slicePitch,
	_Outptr_ DirectX::ScratchImage** const outImage
);

extern "C"
DIRECTXTEXDLL_API HRESULT
APIENTRY
DirectXTex_ScratchImageCreateTexture11(
	_In_ DirectX::ScratchImage* image,
	_In_ ID3D11Device* device,
	_Outptr_ ID3D11Resource** const outTexture
);

