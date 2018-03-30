#pragma once

#include "CSX.h"

namespace CSX
{
	namespace Hook
	{
		PVOID WriteVTable( PVOID pTablePtr , PVOID pFuncAddress , DWORD dwIndex );

		class VTable
		{
		public:

			VTable();
//[swap_lines]
			bool InitTable( PVOID pTablePtrPtr );

			void HookIndex( DWORD dwIndex , PVOID pNewAddress );

			PVOID GetFuncAddress( DWORD dwIndex );
			PVOID GetHookIndex( DWORD dwIndex , PVOID pNewAddress );

			void UnHook();
			void ReHook();
//[/swap_lines]
		private:
//[swap_lines]
			PVOID* pPtrPtrTable;
			PVOID* pPtrOldTable;
			PVOID* pPtrNewTable;

			DWORD dwCountFunc;
			DWORD dwSizeTable;
//[/swap_lines]
		};
	}
}