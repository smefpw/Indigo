#pragma once

namespace SDK
{
	class IDebugOverlay
	{
	public:
		int ScreenPosition( const Vector &vecOrigin, Vector &vecScreen )
		{
			VirtualFn( int )( PVOID, const Vector &, Vector & );
			return GetMethod< OriginalFn >( this, 13 )( this, vecOrigin, vecScreen );
		}
	};
}