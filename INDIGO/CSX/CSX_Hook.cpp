#include "CSX_Hook.h"
//[junk_enable /]
namespace CSX
{
	namespace Hook
	{
		PVOID WriteVTable( PVOID pTablePtr , PVOID pFuncAddress , DWORD dwIndex )
		{
			if ( IsBadReadPtr( pTablePtr , sizeof( PVOID ) ) )
				return nullptr;

			DWORD dwOffset = dwIndex * sizeof( PVOID );
			
			PVOID pFunc = (PVOID)( (DWORD)pTablePtr + dwOffset );
			PVOID Func_o = (PVOID)*(PDWORD)pFunc;

			if ( IsBadReadPtr( pFunc , sizeof( PVOID ) ) )
				return nullptr;

			DWORD dwOld = 0;
			VirtualProtect( pFunc , sizeof( PVOID ) , PAGE_READWRITE , &dwOld );

			*(PDWORD)pFunc = (DWORD)pFuncAddress;

			VirtualProtect( pFunc , sizeof( PVOID ) , dwOld , &dwOld );

			return Func_o;
		}

		VTable::VTable()
		{
			pPtrPtrTable = nullptr;
			pPtrOldTable = nullptr;
			pPtrNewTable = nullptr;
			pPtrPtrTable = nullptr;

			dwCountFunc = 0;
			dwSizeTable = 0;
		}

		bool VTable::InitTable( PVOID pTablePtrPtr )
		{
			if ( IsBadReadPtr( pTablePtrPtr , sizeof( PVOID ) ) )
				return false;

			pPtrPtrTable = (PVOID*)pTablePtrPtr;
			pPtrOldTable = (PVOID*)( *(PDWORD)pPtrPtrTable );

			while ( !CSX::Utils::IsBadReadPtr( pPtrOldTable[dwCountFunc] ) )
				dwCountFunc++;

			//while ( !IsBadCodePtr( (FARPROC)pPtrOldTable[dwCountFunc] ) && !CSX::Utils::IsBadReadPtr( pPtrOldTable[dwCountFunc] ) )
			//	dwCountFunc++;

			if ( dwCountFunc )
			{
				dwSizeTable = sizeof( PVOID ) * dwCountFunc;

				pPtrNewTable = (PVOID*)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , dwSizeTable );
				memcpy( pPtrNewTable , pPtrOldTable , dwSizeTable );

				*(PDWORD)pPtrPtrTable = (DWORD)pPtrNewTable;

				return true;
			}

			return false;
		}

		void VTable::HookIndex( DWORD dwIndex , PVOID pNewAddress )
		{
			if ( pPtrNewTable )
				( (PVOID*)pPtrNewTable )[dwIndex] = pNewAddress;
		}

		PVOID VTable::GetFuncAddress( DWORD dwIndex )
		{
			if ( pPtrOldTable )
			{
				PVOID pAddres = ( (PVOID*)pPtrOldTable )[dwIndex];
				return pAddres;
			}

			return nullptr;
		}

		PVOID VTable::GetHookIndex( DWORD dwIndex , PVOID pNewAddress )
		{
			if ( pPtrNewTable )
			{
				PVOID pOld = ( (PVOID*)pPtrNewTable )[dwIndex];
				( (PVOID*)pPtrNewTable )[dwIndex] = pNewAddress;
				return pOld;
			}

			return nullptr;
		}

		void VTable::UnHook()
		{
			if ( !CSX::Utils::IsBadReadPtr( pPtrPtrTable ) )
				*(PDWORD)pPtrPtrTable = (DWORD)pPtrOldTable;
		}

		void VTable::ReHook()
		{
			if ( !CSX::Utils::IsBadReadPtr( pPtrPtrTable ) )
				*(PDWORD)pPtrPtrTable = (DWORD)pPtrNewTable;
		}
	}
}