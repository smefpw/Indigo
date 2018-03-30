#pragma once

#include "SDK.h"
typedef unsigned short MaterialHandle_t;
namespace SDK
{
	enum ShaderParamType_t
	{
		SHADER_PARAM_TYPE_TEXTURE ,
		SHADER_PARAM_TYPE_INTEGER ,
		SHADER_PARAM_TYPE_COLOR ,
		SHADER_PARAM_TYPE_VEC2 ,
		SHADER_PARAM_TYPE_VEC3 ,
		SHADER_PARAM_TYPE_VEC4 ,
		SHADER_PARAM_TYPE_ENVMAP ,	// obsolete
		SHADER_PARAM_TYPE_FLOAT ,
		SHADER_PARAM_TYPE_BOOL ,
		SHADER_PARAM_TYPE_FOURCC ,
		SHADER_PARAM_TYPE_MATRIX ,
		SHADER_PARAM_TYPE_MATERIAL ,
		SHADER_PARAM_TYPE_STRING ,
	};

	enum MaterialMatrixMode_t
	{
		MATERIAL_VIEW = 0 ,
		MATERIAL_PROJECTION ,

		MATERIAL_MATRIX_UNUSED0 ,
		MATERIAL_MATRIX_UNUSED1 ,
		MATERIAL_MATRIX_UNUSED2 ,
		MATERIAL_MATRIX_UNUSED3 ,
		MATERIAL_MATRIX_UNUSED4 ,
		MATERIAL_MATRIX_UNUSED5 ,
		MATERIAL_MATRIX_UNUSED6 ,
		MATERIAL_MATRIX_UNUSED7 ,

		MATERIAL_MODEL ,

		// Total number of matrices
		NUM_MATRIX_MODES = MATERIAL_MODEL + 1 ,
	};

	// FIXME: How do I specify the actual number of matrix modes?
	const int NUM_MODEL_TRANSFORMS = 53;
	const int MATERIAL_MODEL_MAX = MATERIAL_MODEL + NUM_MODEL_TRANSFORMS;

	enum MaterialPrimitiveType_t
	{
		MATERIAL_POINTS = 0x0 ,
		MATERIAL_LINES ,
		MATERIAL_TRIANGLES ,
		MATERIAL_TRIANGLE_STRIP ,
		MATERIAL_LINE_STRIP ,
		MATERIAL_LINE_LOOP ,	// a single line loop
		MATERIAL_POLYGON ,	// this is a *single* polygon
		MATERIAL_QUADS ,
		MATERIAL_SUBD_QUADS_EXTRA , // Extraordinary sub-d quads
		MATERIAL_SUBD_QUADS_REG ,   // Regular sub-d quads
		MATERIAL_INSTANCED_QUADS , // (X360) like MATERIAL_QUADS, but uses vertex instancing

								   // This is used for static meshes that contain multiple types of
								   // primitive types.	When calling draw, you'll need to specify
								   // a primitive type.
								   MATERIAL_HETEROGENOUS
	};

	enum TessellationMode_t
	{
		TESSELLATION_MODE_DISABLED = 0 ,
		TESSELLATION_MODE_ACC_PATCHES_EXTRA ,
		TESSELLATION_MODE_ACC_PATCHES_REG
	};

	enum MaterialPropertyTypes_t
	{
		MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0 ,					// bool
		MATERIAL_PROPERTY_OPACITY ,								// int (enum MaterialPropertyOpacityTypes_t)
		MATERIAL_PROPERTY_REFLECTIVITY ,							// vec3_t
		MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS				// bool
	};

	// acceptable property values for MATERIAL_PROPERTY_OPACITY
	enum MaterialPropertyOpacityTypes_t
	{
		MATERIAL_ALPHATEST = 0 ,
		MATERIAL_OPAQUE ,
		MATERIAL_TRANSLUCENT
	};

	enum MaterialBufferTypes_t
	{
		MATERIAL_FRONT = 0 ,
		MATERIAL_BACK
	};

	enum MaterialCullMode_t
	{
		MATERIAL_CULLMODE_CCW ,	// this culls polygons with counterclockwise winding
		MATERIAL_CULLMODE_CW	// this culls polygons with clockwise winding
	};

	enum MaterialIndexFormat_t
	{
		MATERIAL_INDEX_FORMAT_UNKNOWN = -1 ,
		MATERIAL_INDEX_FORMAT_16BIT = 0 ,
		MATERIAL_INDEX_FORMAT_32BIT ,
	};

	enum MaterialFogMode_t
	{
		MATERIAL_FOG_NONE ,
		MATERIAL_FOG_LINEAR ,
		MATERIAL_FOG_LINEAR_BELOW_FOG_Z ,
	};

	enum MaterialHeightClipMode_t
	{
		MATERIAL_HEIGHTCLIPMODE_DISABLE ,
		MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT ,
		MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT
	};

	enum MaterialNonInteractiveMode_t
	{
		MATERIAL_NON_INTERACTIVE_MODE_NONE = -1 ,
		MATERIAL_NON_INTERACTIVE_MODE_STARTUP = 0 ,
		MATERIAL_NON_INTERACTIVE_MODE_LEVEL_LOAD ,

		MATERIAL_NON_INTERACTIVE_MODE_COUNT ,
	};

	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------

	enum MaterialThreadMode_t
	{
		MATERIAL_SINGLE_THREADED ,
		MATERIAL_QUEUED_SINGLE_THREADED ,
		MATERIAL_QUEUED_THREADED
	};

	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------

	enum MaterialContextType_t
	{
		MATERIAL_HARDWARE_CONTEXT ,
		MATERIAL_QUEUED_CONTEXT ,
		MATERIAL_NULL_CONTEXT
	};

	enum
	{
		MATERIAL_MAX_LIGHT_COUNT = 4 ,
	};

#define CREATERENDERTARGETFLAGS_HDR				0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP		0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004

	enum MorphFormatFlags_t
	{
		MORPH_POSITION = 0x0001 ,	// 3D
		MORPH_NORMAL = 0x0002 ,	// 3D
		MORPH_WRINKLE = 0x0004 ,	// 1D
		MORPH_SPEED = 0x0008 ,	// 1D
		MORPH_SIDE = 0x0010 ,	// 1D
	};


	//-----------------------------------------------------------------------------
	// The morph format type
	//-----------------------------------------------------------------------------
	typedef unsigned int MorphFormat_t;

	enum StandardLightmap_t
	{
		MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1 ,
		MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2 ,
		MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3
	};


	struct MaterialSystem_SortInfo_t
	{
		IMaterial	*material;
		int			lightmapPageID;
	};


#define MAX_FB_TEXTURES 4

	enum
	{
		MATERIAL_ADAPTER_NAME_LENGTH = 512
	};

	struct MaterialAdapterInfo_t
	{
		char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
		unsigned int m_VendorID;
		unsigned int m_DeviceID;
		unsigned int m_SubSysID;
		unsigned int m_Revision;
		int m_nDXSupportLevel;			// This is the *preferred* dx support level
		int m_nMinDXSupportLevel;
		int m_nMaxDXSupportLevel;
		unsigned int m_nDriverVersionHigh;
		unsigned int m_nDriverVersionLow;
	};

	enum MaterialInitFlags_t
	{
		MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE = 0x2 ,
		MATERIAL_INIT_REFERENCE_RASTERIZER = 0x4 ,
	};

	//-----------------------------------------------------------------------------
	// Flags to specify type of depth buffer used with RT
	//-----------------------------------------------------------------------------

	// GR - this is to add RT with no depth buffer bound

	enum MaterialRenderTargetDepth_t
	{
		MATERIAL_RT_DEPTH_SHARED = 0x0 ,
		MATERIAL_RT_DEPTH_SEPARATE = 0x1 ,
		MATERIAL_RT_DEPTH_NONE = 0x2 ,
		MATERIAL_RT_DEPTH_ONLY = 0x3 ,
	};

	//-----------------------------------------------------------------------------
	// A function to be called when we need to release all vertex buffers
	// NOTE: The restore function will tell the caller if all the vertex formats
	// changed so that it can flush caches, etc. if it needs to (for dxlevel support)
	//-----------------------------------------------------------------------------
	enum RestoreChangeFlags_t
	{
		MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 0x1 ,
		MATERIAL_RESTORE_RELEASE_MANAGED_RESOURCES = 0x2 ,
	};


	// NOTE: All size modes will force the render target to be smaller than or equal to
	// the size of the framebuffer.
	enum RenderTargetSizeMode_t
	{
		RT_SIZE_NO_CHANGE = 0 ,			// Only allowed for render targets that don't want a depth buffer
										// (because if they have a depth buffer, the render target must be less than or equal to the size of the framebuffer).
										RT_SIZE_DEFAULT = 1 ,				// Don't play with the specified width and height other than making sure it fits in the framebuffer.
										RT_SIZE_PICMIP = 2 ,				// Apply picmip to the render target's width and height.
										RT_SIZE_HDR = 3 ,					// frame_buffer_width / 4
										RT_SIZE_FULL_FRAME_BUFFER = 4 ,	// Same size as frame buffer, or next lower power of 2 if we can't do that.
										RT_SIZE_OFFSCREEN = 5 ,			// Target of specified size, don't mess with dimensions
										RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6 // Same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
	};

	struct MaterialTextureInfo_t
	{
		// Exclude information:
		//		-1	texture is not subject to exclude-handling
		//		 0	texture is completely excluded
		//		>0	texture is clamped according to exclude-instruction
		int iExcludeInformation;
	};


	struct ApplicationPerformanceCountersInfo_t
	{
		float msMain;
		float msMST;
		float msGPU;
		float msFlip;
		float msTotal;
	};

	struct ApplicationInstantCountersInfo_t
	{
		uint m_nCpuActivityMask;
		uint m_nDeferredWordsAllocated;
	};

	class IMaterialSystem
	{
	public:
		IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues)
		{
			VirtualFn(IMaterial*)(PVOID, const char*, KeyValues*);
			return GetMethod< OriginalFn >(this, 83)(this, pMaterialName, pVMTKeyValues);
		}

		IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0)
		{
			VirtualFn(IMaterial*)(PVOID, const char*, const char*, bool, const char*);
			return GetMethod< OriginalFn >(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
		}

		MaterialHandle_t FirstMaterial()
		{
			typedef MaterialHandle_t(__thiscall* FirstMaterialFn)(void*);
			return GetMethod <FirstMaterialFn>(this, 86)(this);
		}

		MaterialHandle_t NextMaterial(MaterialHandle_t h)
		{
			typedef MaterialHandle_t(__thiscall* NextMaterialFn)(void*, MaterialHandle_t);
			return GetMethod <NextMaterialFn>(this, 87)(this, h);
		}

		MaterialHandle_t InvalidMaterial()
		{
			typedef MaterialHandle_t(__thiscall* InvalidMaterialFn)(void*);
			return GetMethod <InvalidMaterialFn>(this, 88)(this);
		}

		IMaterial* GetMaterial(MaterialHandle_t h)
		{
			typedef IMaterial*(__thiscall* GetMaterialFn)(void*, MaterialHandle_t);
			return GetMethod <GetMaterialFn>(this, 89)(this, h);
		}
	};
}