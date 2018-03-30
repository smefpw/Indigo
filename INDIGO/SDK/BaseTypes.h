#pragma once

#include <Windows.h>

typedef float vec_t;
typedef int qboolean;

typedef unsigned int			uint;
typedef unsigned char			uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned __int32		uint32;
typedef __int64					int64;
typedef unsigned __int64		uint64;

// In case this ever changes
#ifndef M_PI
#define M_PI			3.14159265358979323846
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( M_PI ) / 180.0f ) )
#endif

inline uint32& FloatBits( vec_t& f )
{
	return *reinterpret_cast<uint32*>( (char*)( &f ) );
}

inline uint32 const FloatBits( const vec_t &f )
{
	union Convertor_t
	{
		vec_t f;
		uint32 ul;
	}tmp;
	tmp.f = f;
	return tmp.ul;
}

inline vec_t BitsToFloat( uint32 i )
{
	union Convertor_t
	{
		vec_t f;
		unsigned long ul;
	}tmp;
	tmp.ul = i;
	return tmp.f;
}

inline bool IsFinite( const vec_t &f )
{
	return ( ( FloatBits( f ) & 0x7F800000 ) != 0x7F800000 );
}

#define FLOAT32_NAN_BITS     (uint32)0x7FC00000	// not a number!
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )

struct vrect_t
{
	int				x , y , width , height;
	vrect_t			*pnext;
};

struct Rect_t
{
	int x , y;
	int width , height;
};

struct colorVec
{
	unsigned r , g , b , a;
};

struct color24
{
	byte r , g , b;
};

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define FORWARD_DECLARE_HANDLE(name) typedef struct name##__ *name

#define DECLARE_DERIVED_POINTER_HANDLE( _name, _basehandle ) struct _name##__ : public _basehandle##__ {}; typedef struct _name##__ *_name
#define DECLARE_ALIASED_POINTER_HANDLE( _name, _alias ) typedef struct _alias##__ *name

#define VEC_T_NAN FLOAT32_NAN

#define MAX_SPLITSCREEN_CLIENT_BITS 2
// this should == MAX_JOYSTICKS in InputEnums.h
#define MAX_SPLITSCREEN_CLIENTS	( 1 << MAX_SPLITSCREEN_CLIENT_BITS ) // 4