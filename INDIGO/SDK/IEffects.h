#pragma once

namespace SDK
{
	enum
	{
		DLIGHT_NO_WORLD_ILLUMINATION = 0x1,
		DLIGHT_NO_MODEL_ILLUMINATION = 0x2,

		// NOTE: These two features are used to dynamically tweak the alpha on displacements
		// which is a special effect for selecting which texture to use. If
		// we ever change how alpha is stored for displacements, we'll have to kill this feature
		DLIGHT_ADD_DISPLACEMENT_ALPHA = 0x4,
		DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA = 0x8,
		DLIGHT_DISPLACEMENT_MASK = (DLIGHT_ADD_DISPLACEMENT_ALPHA | DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA),
	};

	struct ColorRGBExp32
	{
		byte r, g, b;
		signed char exponent;
	};

	struct dlight_t
	{
		int flags;
		Vector origin;
		float radius;
		ColorRGBExp32 color;
		float die;
		float decay;
		float minlight;
		int key;
		int style;
		Vector m_Direction;
		float m_InnerAngle;
		float m_OuterAngle;
		float GetRadius() const
		{
			return radius;
		}
		float GetRadiusSquared() const
		{
			return radius * radius;
		}
		float IsRadiusGreaterThanZero() const
		{
			return radius > 0.0f;
		}
	};

	class CEffects
	{
	public:
		dlight_t* CL_AllocDlight(int key)
		{
			typedef dlight_t*(__thiscall* FindVarFn)(PVOID, int);
			return GetMethod<FindVarFn>(this, 4)(this, key);
		}
		dlight_t* CL_AllocElight(int key)
		{
			typedef dlight_t*(__thiscall* FindVarFn)(PVOID, int);
			return GetMethod<FindVarFn>(this, 5)(this, key);
		}
		dlight_t* GetElightByKey(int key)
		{
			typedef dlight_t*(__thiscall* FindVarFn)(PVOID, int);
			return GetMethod<FindVarFn>(this, 8)(this, key);
		}
	};
}
