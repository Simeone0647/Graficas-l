#pragma once
#define SIMECOMPILE_DEBUG (1 << 0)
#define SIME_PIDIV4           0.785398163f
#define SIMECOMPILE_ENABLE_STRICTNESS              (1 << 11)
#define SIME_USAGE_RENDER_TARGET_OUTPUT     ( 1L << (1 + 4) )
#define SIME_COLOR_BUFFER_BIT 0x00004000
#define SIME_DEPTH_BUFFER_BIT 0x00000100
#define SIME_TRIANGLES 0x0004
#define SIME_UNSIGNED_INT 0x1405
#define SIME_UNSIGNED_SHORT 0x1403
#define SIME_ARRAY_BUFFER 0x8892
#define SIME_ELEMENT_ARRAY_BUFFER 0x8893
#define SIME_STATIC_DRAW 0x88E4
#define SIME_FLOAT 0x1406

#include <windows.h>
#include "Buffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Viewport.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "Manager.h"
#include "RenderTargetView.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vertex.h"
#include "InputLayout.h"
#include "ShaderReflection.h"
#include "SamplerState.h"
#include "ShaderResourceView.h"
#include "Matrix.h"
#include <string>
#include <fstream>
#include <sstream>

#if defined(OGL)
#include "FreeImage.h"
#include <glm.hpp>
#include <glad/glad.h>
#include <glfw3.h>
#include <gtx/transform.hpp>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#endif
#if defined(DX11)
#include "FreeImage.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
namespace GraphicsModule
{
	enum SIME_RESOURCE_MISC_FLAG
	{
		SIME_RESOURCE_MISC_GENERATE_MIPS = 0x1L,
		SIME_RESOURCE_MISC_SHARED = 0x2L,
		SIME_RESOURCE_MISC_TEXTURECUBE = 0x4L,
		SIME_RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
		SIME_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
		SIME_RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
		SIME_RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
		SIME_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
		SIME_RESOURCE_MISC_GDI_COMPATIBLE = 0x200L
	};

	enum SIME_PRIMITIVE_TOPOLOGY
	{
		SIME_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
		SIME_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
		SIME_PRIMITIVE_TOPOLOGY_LINELIST = 2,
		SIME_PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
		SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
		SIME_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
		SIME_PRIMITIVE_TOPOLOGY_LINELIST_ADJ = 10,
		SIME_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = 11,
		SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
		SIME_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
		SIME_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = 33,
		SIME_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = 34,
		SIME_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35,
		SIME_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = 36,
		SIME_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = 37,
		SIME_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = 38,
		SIME_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = 39,
		SIME_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = 40,
		SIME_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = 41,
		SIME_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
		SIME_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
		SIME_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
		SIME_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
		SIME_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
		SIME_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
		SIME_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
		SIME_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
		SIME_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
		SIME_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
		SIME_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
		SIME_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
		SIME_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
		SIME_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
		SIME_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
		SIME_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
		SIME_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
		SIME_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
		SIME_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
		SIME_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
		SIME_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
		SIME_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
		SIME_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64,
		SIME10_PRIMITIVE_TOPOLOGY_UNDEFINED = SIME_PRIMITIVE_TOPOLOGY_UNDEFINED,
		SIME10_PRIMITIVE_TOPOLOGY_POINTLIST = SIME_PRIMITIVE_TOPOLOGY_POINTLIST,
		SIME10_PRIMITIVE_TOPOLOGY_LINELIST = SIME_PRIMITIVE_TOPOLOGY_LINELIST,
		SIME10_PRIMITIVE_TOPOLOGY_LINESTRIP = SIME_PRIMITIVE_TOPOLOGY_LINESTRIP,
		SIME10_PRIMITIVE_TOPOLOGY_TRIANGLELIST = SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		SIME10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = SIME_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		SIME10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ = SIME_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
		SIME10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = SIME_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
		SIME10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
		SIME10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = SIME_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
		SIME11_PRIMITIVE_TOPOLOGY_UNDEFINED = SIME_PRIMITIVE_TOPOLOGY_UNDEFINED,
		SIME11_PRIMITIVE_TOPOLOGY_POINTLIST = SIME_PRIMITIVE_TOPOLOGY_POINTLIST,
		SIME11_PRIMITIVE_TOPOLOGY_LINELIST = SIME_PRIMITIVE_TOPOLOGY_LINELIST,
		SIME11_PRIMITIVE_TOPOLOGY_LINESTRIP = SIME_PRIMITIVE_TOPOLOGY_LINESTRIP,
		SIME11_PRIMITIVE_TOPOLOGY_TRIANGLELIST = SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		SIME11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = SIME_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		SIME11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ = SIME_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
		SIME11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = SIME_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
		SIME11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
		SIME11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = SIME_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
		SIME11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
		SIME11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = SIME_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST
	};

	enum SIME_CLEAR_FLAG
	{
		SIME_CLEAR_DEPTH = 0x1L,
		SIME_CLEAR_STENCIL = 0x2L
	};

	enum SIME_SRV_DIMENSION
	{
		SIME_SRV_DIMENSION_UNKNOWN = 0,
		SIME_SRV_DIMENSION_BUFFER = 1,
		SIME_SRV_DIMENSION_TEXTURE1D = 2,
		SIME_SRV_DIMENSION_TEXTURE1DARRAY = 3,
		SIME_SRV_DIMENSION_TEXTURE2D = 4,
		SIME_SRV_DIMENSION_TEXTURE2DARRAY = 5,
		SIME_SRV_DIMENSION_TEXTURE2DMS = 6,
		SIME_SRV_DIMENSION_TEXTURE2DMSARRAY = 7,
		SIME_SRV_DIMENSION_TEXTURE3D = 8,
		SIME_SRV_DIMENSION_TEXTURECUBE = 9,
		SIME_SRV_DIMENSION_TEXTURECUBEARRAY = 10,
		SIME_SRV_DIMENSION_BUFFEREX = 11,
		SIME10_SRV_DIMENSION_UNKNOWN = SIME_SRV_DIMENSION_UNKNOWN,
		SIME10_SRV_DIMENSION_BUFFER = SIME_SRV_DIMENSION_BUFFER,
		SIME10_SRV_DIMENSION_TEXTURE1D = SIME_SRV_DIMENSION_TEXTURE1D,
		SIME10_SRV_DIMENSION_TEXTURE1DARRAY = SIME_SRV_DIMENSION_TEXTURE1DARRAY,
		SIME10_SRV_DIMENSION_TEXTURE2D = SIME_SRV_DIMENSION_TEXTURE2D,
		SIME10_SRV_DIMENSION_TEXTURE2DARRAY = SIME_SRV_DIMENSION_TEXTURE2DARRAY,
		SIME10_SRV_DIMENSION_TEXTURE2DMS = SIME_SRV_DIMENSION_TEXTURE2DMS,
		SIME10_SRV_DIMENSION_TEXTURE2DMSARRAY = SIME_SRV_DIMENSION_TEXTURE2DMSARRAY,
		SIME10_SRV_DIMENSION_TEXTURE3D = SIME_SRV_DIMENSION_TEXTURE3D,
		SIME10_SRV_DIMENSION_TEXTURECUBE = SIME_SRV_DIMENSION_TEXTURECUBE,
		SIME10_1_SRV_DIMENSION_UNKNOWN = SIME_SRV_DIMENSION_UNKNOWN,
		SIME10_1_SRV_DIMENSION_BUFFER = SIME_SRV_DIMENSION_BUFFER,
		SIME10_1_SRV_DIMENSION_TEXTURE1D = SIME_SRV_DIMENSION_TEXTURE1D,
		SIME10_1_SRV_DIMENSION_TEXTURE1DARRAY = SIME_SRV_DIMENSION_TEXTURE1DARRAY,
		SIME10_1_SRV_DIMENSION_TEXTURE2D = SIME_SRV_DIMENSION_TEXTURE2D,
		SIME10_1_SRV_DIMENSION_TEXTURE2DARRAY = SIME_SRV_DIMENSION_TEXTURE2DARRAY,
		SIME10_1_SRV_DIMENSION_TEXTURE2DMS = SIME_SRV_DIMENSION_TEXTURE2DMS,
		SIME10_1_SRV_DIMENSION_TEXTURE2DMSARRAY = SIME_SRV_DIMENSION_TEXTURE2DMSARRAY,
		SIME10_1_SRV_DIMENSION_TEXTURE3D = SIME_SRV_DIMENSION_TEXTURE3D,
		SIME10_1_SRV_DIMENSION_TEXTURECUBE = SIME_SRV_DIMENSION_TEXTURECUBE,
		SIME10_1_SRV_DIMENSION_TEXTURECUBEARRAY = SIME_SRV_DIMENSION_TEXTURECUBEARRAY,
		SIME11_SRV_DIMENSION_UNKNOWN = SIME_SRV_DIMENSION_UNKNOWN,
		SIME11_SRV_DIMENSION_BUFFER = SIME_SRV_DIMENSION_BUFFER,
		SIME11_SRV_DIMENSION_TEXTURE1D = SIME_SRV_DIMENSION_TEXTURE1D,
		SIME11_SRV_DIMENSION_TEXTURE1DARRAY = SIME_SRV_DIMENSION_TEXTURE1DARRAY,
		SIME11_SRV_DIMENSION_TEXTURE2D = SIME_SRV_DIMENSION_TEXTURE2D,
		SIME11_SRV_DIMENSION_TEXTURE2DARRAY = SIME_SRV_DIMENSION_TEXTURE2DARRAY,
		SIME11_SRV_DIMENSION_TEXTURE2DMS = SIME_SRV_DIMENSION_TEXTURE2DMS,
		SIME11_SRV_DIMENSION_TEXTURE2DMSARRAY = SIME_SRV_DIMENSION_TEXTURE2DMSARRAY,
		SIME11_SRV_DIMENSION_TEXTURE3D = SIME_SRV_DIMENSION_TEXTURE3D,
		SIME11_SRV_DIMENSION_TEXTURECUBE = SIME_SRV_DIMENSION_TEXTURECUBE,
		SIME11_SRV_DIMENSION_TEXTURECUBEARRAY = SIME_SRV_DIMENSION_TEXTURECUBEARRAY,
		SIME11_SRV_DIMENSION_BUFFEREX = SIME_SRV_DIMENSION_BUFFEREX
	};

	enum SIME_DRIVER_TYPE
	{
		SIME_DRIVER_TYPE_UNKNOWN = 0,
		SIME_DRIVER_TYPE_HARDWARE = (SIME_DRIVER_TYPE_UNKNOWN + 1),
		SIME_DRIVER_TYPE_REFERENCE = (SIME_DRIVER_TYPE_HARDWARE + 1),
		SIME_DRIVER_TYPE_NULL = (SIME_DRIVER_TYPE_REFERENCE + 1),
		SIME_DRIVER_TYPE_SOFTWARE = (SIME_DRIVER_TYPE_NULL + 1),
		SIME_DRIVER_TYPE_WARP = (SIME_DRIVER_TYPE_SOFTWARE + 1)
	};

	enum SIME_FEATURE_LEVEL
	{
		SIME_FEATURE_LEVEL_9_1 = 0x9100,
		SIME_FEATURE_LEVEL_9_2 = 0x9200,
		SIME_FEATURE_LEVEL_9_3 = 0x9300,
		SIME_FEATURE_LEVEL_10_0 = 0xa000,
		SIME_FEATURE_LEVEL_10_1 = 0xa100,
		SIME_FEATURE_LEVEL_11_0 = 0xb000
	};

	enum SIME_CREATE_DEVICE_FLAG
	{
		SIME_CREATE_DEVICE_SINGLETHREADED = 0x1,
		SIME_CREATE_DEVICE_DEBUG = 0x2,
		SIME_CREATE_DEVICE_SWITCH_TO_REF = 0x4,
		SIME_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8,
		SIME_CREATE_DEVICE_BGRA_SUPPORT = 0x20
	};

	enum SIME_FORMAT
	{
		SIME_FORMAT_UNKNOWN = 0,
		SIME_FORMAT_R32G32B32A32_TYPELESS = 1,
		SIME_FORMAT_R32G32B32A32_FLOAT = 2,
		SIME_FORMAT_R32G32B32A32_UINT = 3,
		SIME_FORMAT_R32G32B32A32_SINT = 4,
		SIME_FORMAT_R32G32B32_TYPELESS = 5,
		SIME_FORMAT_R32G32B32_FLOAT = 6,
		SIME_FORMAT_R32G32B32_UINT = 7,
		SIME_FORMAT_R32G32B32_SINT = 8,
		SIME_FORMAT_R16G16B16A16_TYPELESS = 9,
		SIME_FORMAT_R16G16B16A16_FLOAT = 10,
		SIME_FORMAT_R16G16B16A16_UNORM = 11,
		SIME_FORMAT_R16G16B16A16_UINT = 12,
		SIME_FORMAT_R16G16B16A16_SNORM = 13,
		SIME_FORMAT_R16G16B16A16_SINT = 14,
		SIME_FORMAT_R32G32_TYPELESS = 15,
		SIME_FORMAT_R32G32_FLOAT = 16,
		SIME_FORMAT_R32G32_UINT = 17,
		SIME_FORMAT_R32G32_SINT = 18,
		SIME_FORMAT_R32G8X24_TYPELESS = 19,
		SIME_FORMAT_D32_FLOAT_S8X24_UINT = 20,
		SIME_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
		SIME_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
		SIME_FORMAT_R10G10B10A2_TYPELESS = 23,
		SIME_FORMAT_R10G10B10A2_UNORM = 24,
		SIME_FORMAT_R10G10B10A2_UINT = 25,
		SIME_FORMAT_R11G11B10_FLOAT = 26,
		SIME_FORMAT_R8G8B8A8_TYPELESS = 27,
		SIME_FORMAT_R8G8B8A8_UNORM = 28,
		SIME_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
		SIME_FORMAT_R8G8B8A8_UINT = 30,
		SIME_FORMAT_R8G8B8A8_SNORM = 31,
		SIME_FORMAT_R8G8B8A8_SINT = 32,
		SIME_FORMAT_R16G16_TYPELESS = 33,
		SIME_FORMAT_R16G16_FLOAT = 34,
		SIME_FORMAT_R16G16_UNORM = 35,
		SIME_FORMAT_R16G16_UINT = 36,
		SIME_FORMAT_R16G16_SNORM = 37,
		SIME_FORMAT_R16G16_SINT = 38,
		SIME_FORMAT_R32_TYPELESS = 39,
		SIME_FORMAT_D32_FLOAT = 40,
		SIME_FORMAT_R32_FLOAT = 41,
		SIME_FORMAT_R32_UINT = 42,
		SIME_FORMAT_R32_SINT = 43,
		SIME_FORMAT_R24G8_TYPELESS = 44,
		SIME_FORMAT_D24_UNORM_S8_UINT = 45,
		SIME_FORMAT_R24_UNORM_X8_TYPELESS = 46,
		SIME_FORMAT_X24_TYPELESS_G8_UINT = 47,
		SIME_FORMAT_R8G8_TYPELESS = 48,
		SIME_FORMAT_R8G8_UNORM = 49,
		SIME_FORMAT_R8G8_UINT = 50,
		SIME_FORMAT_R8G8_SNORM = 51,
		SIME_FORMAT_R8G8_SINT = 52,
		SIME_FORMAT_R16_TYPELESS = 53,
		SIME_FORMAT_R16_FLOAT = 54,
		SIME_FORMAT_D16_UNORM = 55,
		SIME_FORMAT_R16_UNORM = 56,
		SIME_FORMAT_R16_UINT = 57,
		SIME_FORMAT_R16_SNORM = 58,
		SIME_FORMAT_R16_SINT = 59,
		SIME_FORMAT_R8_TYPELESS = 60,
		SIME_FORMAT_R8_UNORM = 61,
		SIME_FORMAT_R8_UINT = 62,
		SIME_FORMAT_R8_SNORM = 63,
		SIME_FORMAT_R8_SINT = 64,
		SIME_FORMAT_A8_UNORM = 65,
		SIME_FORMAT_R1_UNORM = 66,
		SIME_FORMAT_R9G9B9E5_SHAREDEXP = 67,
		SIME_FORMAT_R8G8_B8G8_UNORM = 68,
		SIME_FORMAT_G8R8_G8B8_UNORM = 69,
		SIME_FORMAT_BC1_TYPELESS = 70,
		SIME_FORMAT_BC1_UNORM = 71,
		SIME_FORMAT_BC1_UNORM_SRGB = 72,
		SIME_FORMAT_BC2_TYPELESS = 73,
		SIME_FORMAT_BC2_UNORM = 74,
		SIME_FORMAT_BC2_UNORM_SRGB = 75,
		SIME_FORMAT_BC3_TYPELESS = 76,
		SIME_FORMAT_BC3_UNORM = 77,
		SIME_FORMAT_BC3_UNORM_SRGB = 78,
		SIME_FORMAT_BC4_TYPELESS = 79,
		SIME_FORMAT_BC4_UNORM = 80,
		SIME_FORMAT_BC4_SNORM = 81,
		SIME_FORMAT_BC5_TYPELESS = 82,
		SIME_FORMAT_BC5_UNORM = 83,
		SIME_FORMAT_BC5_SNORM = 84,
		SIME_FORMAT_B5G6R5_UNORM = 85,
		SIME_FORMAT_B5G5R5A1_UNORM = 86,
		SIME_FORMAT_B8G8R8A8_UNORM = 87,
		SIME_FORMAT_B8G8R8X8_UNORM = 88,
		SIME_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
		SIME_FORMAT_B8G8R8A8_TYPELESS = 90,
		SIME_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
		SIME_FORMAT_B8G8R8X8_TYPELESS = 92,
		SIME_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
		SIME_FORMAT_BC6H_TYPELESS = 94,
		SIME_FORMAT_BC6H_UF16 = 95,
		SIME_FORMAT_BC6H_SF16 = 96,
		SIME_FORMAT_BC7_TYPELESS = 97,
		SIME_FORMAT_BC7_UNORM = 98,
		SIME_FORMAT_BC7_UNORM_SRGB = 99,
		SIME_FORMAT_FORCE_UINT = 0xffffffff
	};

	enum SIME_USAGE
	{
		SIME_USAGE_DEFAULT = 0,
		SIME_USAGE_IMMUTABLE = 1,
		SIME_USAGE_DYNAMIC = 2,
		SIME_USAGE_STAGING = 3
	};

	enum SIME_BIND_FLAG
	{
		SIME_BIND_VERTEX_BUFFER = 0x1L,
		SIME_BIND_INDEX_BUFFER = 0x2L,
		SIME_BIND_CONSTANT_BUFFER = 0x4L,
		SIME_BIND_SHADER_RESOURCE = 0x8L,
		SIME_BIND_STREAM_OUTPUT = 0x10L,
		SIME_BIND_RENDER_TARGET = 0x20L,
		SIME_BIND_DEPTH_STENCIL = 0x40L,
		SIME_BIND_UNORDERED_ACCESS = 0x80L
	};

	enum SIME_DSV_DIMENSION
	{
		SIME_DSV_DIMENSION_UNKNOWN = 0,
		SIME_DSV_DIMENSION_TEXTURE1D = 1,
		SIME_DSV_DIMENSION_TEXTURE1DARRAY = 2,
		SIME_DSV_DIMENSION_TEXTURE2D = 3,
		SIME_DSV_DIMENSION_TEXTURE2DARRAY = 4,
		SIME_DSV_DIMENSION_TEXTURE2DMS = 5,
		SIME_DSV_DIMENSION_TEXTURE2DMSARRAY = 6
	};
//---------------------------STRUCTS------------------------------------

	struct SimpleVertex
	{
	#if defined(DX11)
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Normal;
	#endif
	};

	struct DirLight
	{
	#if defined(DX11)
		XMFLOAT4 Dir;
	#endif
	#if defined(OGL)
		float Dir[4];
	#endif
	};

	struct CBNeverChanges
	{
	#if defined(DX11)
		XMMATRIX mView;
		//Matrix mView;
	#endif
	};

	struct CBChangeOnResize
	{
	#if defined(DX11)
		XMMATRIX mProjection;
		//Matrix mProjection;
	#endif
	};

	//struct CBChangesEveryFrame
	//{
	//#if defined(DX11)
	//	XMMATRIX mWorld;
	//	XMFLOAT4 vMeshColor;
	//#endif
	//};

	struct CreateDevAndSCDesc
	{
	#if defined(DX11)
		SIME_DRIVER_TYPE driverType;
		UINT CreateDeviceFlags;
		SIME_FEATURE_LEVEL FeatureLevels[3];
		UINT NumFeatureLevels;
		UINT SDKVersion;
		DXGI_SWAP_CHAIN_DESC sd;
		SIME_FEATURE_LEVEL FeatureLevel;
	#endif
	};

	struct CreateDepthDesc
	{
	#if defined(DX11)
		UINT Width;
		UINT Height;
		UINT Mips;
		UINT ArraySize;
		SIME_FORMAT Format;
		UINT Count;
		UINT Quality;
		SIME_USAGE Usage;
		UINT BindFlags;
		UINT CPUAccessFlags;
		UINT MiscFlags;
	#endif
	};

	struct InitViewportStruct
	{
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
		float TopLeftX;
		float TopLeftY;
	};

	struct CreateVertexShaderStruct
	{
	#if defined(DX11)
		const void* pShaderBytecode;
		SIZE_T BytecodeLength;
		ID3D11ClassLinkage* pClassLinkage;
		ID3D11VertexShader** ppVertexShader;
	#endif
	};

	struct CreateInputLayoutStruct
	{
	#if defined(DX11)
		__in_ecount(NumElements) const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs;
		__in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT) UINT NumElements;
		__in const void* pShaderBytecodeWithInputSignature;
		__in SIZE_T BytecodeLength;
		__out_opt ID3D11InputLayout** ppInputLayout;
	#endif
	};

	struct CreatePixelShaderStruct
	{
	#if defined(DX11)
		__in  const void* pShaderBytecode;
		__in  SIZE_T BytecodeLength;
		__in_opt  ID3D11ClassLinkage* pClassLinkage;
		__out_opt  ID3D11PixelShader** ppPixelShader;
	#endif
	};

	struct UpdateBDStruct
	{
		SIME_USAGE Usage;
		UINT ByteWidth;
		UINT BindFlags;
		UINT CPUAccessFlags;
		UINT MiscFlags;
		UINT StructureBytestride;
	};

	struct SetRTDescStruct
	{
		UINT Width;
		UINT Height;
		UINT MipLevels;
		UINT Arraysize;
		SIME_FORMAT Format;
		UINT Count;
		UINT Quality;
		SIME_USAGE Usage;
		UINT CPUAccessFlags;
		UINT MiscFlags;
		UINT BindFlags;
	};

	struct UpdateProjectionMatrixStruct
	{
		float AngleY;
		float Ratio;
		float NearPlane;
		float FarPlane;
		float Width;
		float Height;
	};

	struct UpdateSubResourceStruct
	{
	#if defined(DX11)
		__in  ID3D11Resource* pDstResource;
		__in  UINT DstSubresource;
		__in_opt  const D3D11_BOX* pDstBox;
		__in  const void* pSrcData;
		__in  UINT SrcRowPitch;
		__in  UINT SrcDepthPitch;
	#endif
	};

	struct ClearDepthStencilViewStruct
	{
	#if defined(DX11)
		__in  ID3D11DepthStencilView* pDepthStencilView;
		__in  UINT ClearFlags;
		__in FLOAT Depth;
		__in  UINT8 Stencil;
	#endif
	};

	struct SetVertexBufferStruct
	{
	#if defined(DX11)
		__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot;
		__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers;
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers;
		__in_ecount(NumBuffers)  const UINT* pStrides;
		__in_ecount(NumBuffers)  const UINT* pOffsets;
	#endif
	};

	class test
	{
	public:

		unsigned int programID = 0;

		CreateDepthDesc						g_DepthDesc;
		HWND                                g_hWnd = NULL;
		ShaderResourceView					g_SimeTextureRV;
		ShaderResourceView					g_SimeDepthStencilSRV;
		SamplerState						g_SimeSamplerState;
		PixelShader							g_SimePixelShader;
		ShaderReflection					g_SimeVertexShaderReflection;
		InputLayout							g_SimeInputLayout;
		VertexShader						g_SimeVertexShader;
		DirLight							g_DirLightBufferDesc;
		Buffer								g_DirLightBuffer;
		VertexBuffer						g_SimeVertexBuffer;
		IndexBuffer							g_SimeIndexBuffer;
		ConstantBuffer						g_SimeCBNeverChanges;
		ConstantBuffer						g_SimeCBChangeOnResize;
		ConstantBuffer						g_SimeCBChangesEveryFrame;
		Viewport							g_SimeViewport;
		Texture2D							g_SimeDepthStencil;
		Texture2D							g_TextureRenderTarget;
		Texture2D							g_TextureRenderTarget2;
		Texture2D							g_TextureRenderTarget3;
		CreateDevAndSCDesc					g_DeviceAndSwapChainDesc;
		DepthStencilView					g_SimeDepthStencilView;
		RenderTargetView					g_SimeRenderTargetView;
		SIME_DRIVER_TYPE                    g_driverType = SIME_DRIVER_TYPE_NULL;
		Camera                              m_PerspectiveCamera;
		Camera                              m_OrtographicCamera;
		Camera*								m_Camera;
		LPPOINT                             MouseInitPos = new POINT;
		LPPOINT                             MouseFinalPos = new POINT;
		LPPOINT                             MouseDirection = new POINT;
		unsigned int* CubeVertexIndex =   new unsigned int[36];
		bool                                m_IsPerspectiveActive = true;
		bool                                m_IsFirstFrame = false;
		bool                                m_IsFirstPosStored = false;
		bool								m_ShowingTexture = false;

	#if defined(DX11)
		HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	#endif

	#if defined(OGL)
		GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	#endif
	public:
		HRESULT InitDevice(HWND hdwn);
	
		void Render();
	
		void Update();
		
	#if defined(OGL)
		void UpdateOGL(GLFWwindow* _window);
	#endif
		void CleanupDevice();

		HWND m_hwnd;
	};

	extern Manager& GetManagerObj(HWND hwnd);
}