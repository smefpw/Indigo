#pragma once

#include "Definitions.hpp"
#include "PlayerInfo.hpp"
#include "Vector.hpp"

//20th october 2020 - (12th November 2020)
namespace SDK {
	class GlowObjectDefinition_t {
	public:
		GlowObjectDefinition_t() { memset(this, 0, sizeof(*this)); } //wat is this???

		void set(float r, float g, float b, float a) {
			m_vGlowColor = Vector(r, g, b);
			m_flAlpha = a;
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
		}
		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;

		/*
		//8th july 2021:
		New Glow Structure Offsets

		[Float] Red Offset: (Index * 0x38) + 0x8
		[Float] Green Offset: (Index * 0x38) + 0xC
		[Float] Blue Offset: (Index * 0x38) + 0x10
		[Float] Alpha Offset: (Index * 0x38) + 0x14

		[Bool] Static Offset: (Index * 0x38) + 0x28
		[Bool] Pulsing Offset: (Index * 0x38) + 0x30
		[Bool] Full Offset: (Index * 0x38) + 0x2A
		*/

		//17th july 2021 - m_nNextFreeSlot moved to start
		int		m_nNextFreeSlot;           //0x0000 - int
		class IClientEntity* m_pEntity;    //0x0004 - void* entity
		union {
			Vector m_vGlowColor;		   //0x0008 - vec3_t(r, g, b)
			struct {
				float   m_flRed;           //0x000C
				float   m_flGreen;         //0x0010
				float   m_flBlue;          //0x0014
			};
		};
		float   m_flAlpha;                 //0x0018 - float alpha
		unsigned char unk1[0x10];          //0x0028
		bool    m_bRenderWhenOccluded;     //0x0029
		bool    m_bRenderWhenUnoccluded;   //0x0030
		bool    m_bFullBloomRender;        //0x0031
		unsigned char unk2[0xA];           //0x003B
		//after this could be wrong !! 
		/*uint8_t pad_0027[5];           //0x0027 - char
		int32_t m_nGlowStyle;			   //0x002C - ??
		int m_nSplitScreenSlot;            //0x0030 - ???
		*/

	}; //Size: 0x0038 (56)

	class CGlowObjectManager {
	public:
		int RegisterGlowObject(IClientEntity *pEntity, const Vector &vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot) {
			int nIndex;
			if (m_nFirstFreeSlot == GlowObjectDefinition_t::END_OF_FREE_LIST) {
				nIndex = m_GlowObjectDefinitions.AddToTail();
			}
			else {
				nIndex = m_nFirstFreeSlot;
				m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
			}

			//17th july 2021 - m_nNextFreeSlot moved to start
			m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot = GlowObjectDefinition_t::ENTRY_IN_USE;
			m_GlowObjectDefinitions[nIndex].m_pEntity = pEntity;
			m_GlowObjectDefinitions[nIndex].m_vGlowColor = vGlowColor;
			m_GlowObjectDefinitions[nIndex].m_flAlpha = flGlowAlpha;
			m_GlowObjectDefinitions[nIndex].m_bRenderWhenOccluded = bRenderWhenOccluded;
			m_GlowObjectDefinitions[nIndex].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
			//m_GlowObjectDefinitions[nIndex].m_nSplitScreenSlot = nSplitScreenSlot; //fix if u cba lol

			return nIndex;
		}

		void UnregisterGlowObject(int nGlowObjectHandle) {
			m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
			m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = NULL;
			m_nFirstFreeSlot = nGlowObjectHandle;
		}

		void SetEntity(int nGlowObjectHandle, IClientEntity *pEntity) {
			m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = pEntity;
		}

		void SetColor(int nGlowObjectHandle, const Vector &vGlowColor) {
			m_GlowObjectDefinitions[nGlowObjectHandle].m_vGlowColor = vGlowColor;
		}

		void SetAlpha(int nGlowObjectHandle, float flAlpha) {
			m_GlowObjectDefinitions[nGlowObjectHandle].m_flAlpha = flAlpha;
		}

		void SetRenderFlags(int nGlowObjectHandle, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded) {
			m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenOccluded = bRenderWhenOccluded;
			m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
		}

		bool IsRenderingWhenOccluded(int nGlowObjectHandle) const {
			return m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenOccluded;
		}

		bool IsRenderingWhenUnoccluded(int nGlowObjectHandle) const {
			return m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenUnoccluded;
		}

		bool HasGlowEffect(IClientEntity *pEntity) const {
			for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i) {
				if (!m_GlowObjectDefinitions[i].IsUnused() && m_GlowObjectDefinitions[i].m_pEntity == pEntity) {
					return true;
				}
			}

			return false;
		}


		CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions; //0x0000
		int m_nFirstFreeSlot;                                       //0x000C
	};
}