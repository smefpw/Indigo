#pragma once

#include "Definitions.hpp"
#include "VMatrix.hpp"

namespace SDK
{
	class IPanel;
	class C_BaseEntity;

	enum class ClearFlags_t {
		VIEW_CLEAR_COLOR = 0x1 ,
		VIEW_CLEAR_DEPTH = 0x2 ,
		VIEW_CLEAR_FULL_TARGET = 0x4 ,
		VIEW_NO_DRAW = 0x8 ,
		VIEW_CLEAR_OBEY_STENCIL = 0x10 ,
		VIEW_CLEAR_STENCIL = 0x20 ,
	};

	enum class MotionBlurMode_t {
		MOTION_BLUR_DISABLE = 1 ,
		MOTION_BLUR_GAME = 2 ,
		MOTION_BLUR_SFM = 3
	};

	class CViewSetup
	{
	public:
		// left side of view window
		int			x , x_old;
		// top side of view window
		int			y , y_old;
		// width of view window
		int			width , width_old;
		// height of view window
		int			height , height_old;
		// the rest are only used by 3D views
		// Orthographic projection?
		bool		m_bOrtho;
		// View-space rectangle for ortho projection.
		float		m_OrthoLeft;
		float		m_OrthoTop;
		float		m_OrthoRight;
		float		m_OrthoBottom;
		bool		m_bCustomViewMatrix; // 0x34
		matrix3x4_t	m_matCustomViewMatrix; // 0x38
		char		pad_0x68[0x48]; // 0x68
									// horizontal FOV in degrees
		float		fov; //	0xB0
						 // horizontal FOV in degrees for in-view model
		float		fovViewmodel;
		// 3D origin of camera
		Vector		origin;
		// heading of camera (pitch, yaw, roll)
		QAngle		angles;
		// local Z coordinate of near plane of camera
		float		zNear;
		// local Z coordinate of far plane of camera
		float		zFar;
		// local Z coordinate of near plane of camera ( when rendering view model )
		float		zNearViewmodel;
		// local Z coordinate of far plane of camera ( when rendering view model )
		float		zFarViewmodel;
		// The aspect ratio to use for computing the perspective projection matrix
		// (0.0f means use the viewport)
		float		m_flAspectRatio;
		// Camera settings to control depth of field
		float		m_flNearBlurDepth;
		float		m_flNearFocusDepth;
		float		m_flFarFocusDepth;
		float		m_flFarBlurDepth;
		float		m_flNearBlurRadius;
		float		m_flFarBlurRadius;
		int			m_nDoFQuality;
		// Camera settings to control motion blur
		MotionBlurMode_t	m_nMotionBlurMode;
		float		m_flShutterTime;				// In seconds
		Vector		m_vShutterOpenPosition;			// Start of frame or "shutter open"
		QAngle		m_shutterOpenAngles;			//
		Vector		m_vShutterClosePosition;		// End of frame or "shutter close"
		QAngle		m_shutterCloseAngles;			// 
													// Controls for off-center projection (needed for poster rendering)
		float		m_flOffCenterTop;
		float		m_flOffCenterBottom;
		float		m_flOffCenterLeft;
		float		m_flOffCenterRight;
		int			m_EdgeBlur; // 0x148
	};

	namespace TABLE
	{
		namespace IClientMode
		{
			enum
			{
				OverrideView = 18 ,
				CreateMove = 24 ,
				GetViewModelFOV = 35
			};
		}
	}

	class IClientMode {
	public:
		bool OverrideView( CViewSetup* pSetup )
		{
			VirtualFn( bool )( PVOID , CViewSetup* );
			return GetMethod< OriginalFn >( this , TABLE::IClientMode::OverrideView )( this , pSetup );
		}
		bool CreateMove( float flInputSampleTime , CUserCmd* cmd )
		{
			VirtualFn( bool )( PVOID , float , CUserCmd* );
			return GetMethod< OriginalFn >( this , TABLE::IClientMode::CreateMove )( this , flInputSampleTime , cmd );
		}
		float GetViewModelFOV()
		{
			VirtualFn( float )( PVOID );
			return GetMethod< OriginalFn >( this , TABLE::IClientMode::GetViewModelFOV )( this );
		}
	};
}