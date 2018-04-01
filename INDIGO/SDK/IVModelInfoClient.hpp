#pragma once

#include "Definitions.hpp"

#include "Vector.hpp"
#include "VMatrix.hpp"

class CUtlBuffer;
struct virtualmodel_t;
typedef unsigned short MDLHandle_t;

namespace SDK
{
	enum HitboxEnum_t {
		HITBOX_HEAD ,
		HITBOX_NECK ,
		HITBOX_LOWER_NECK ,
		HITBOX_PELVIS_REMOVED ,
		HITBOX_BODY ,
		HITBOX_THORAX ,
		HITBOX_CHEST ,
		HITBOX_UPPER_CHEST ,
		HITBOX_RIGHT_THIGH ,
		HITBOX_LEFT_THIGH ,
		HITBOX_RIGHT_CALF ,
		HITBOX_LEFT_CALF ,
		HITBOX_RIGHT_FOOT ,
		HITBOX_LEFT_FOOT ,
		HITBOX_RIGHT_HAND ,
		HITBOX_LEFT_HAND ,
		HITBOX_RIGHT_UPPER_ARM ,
		HITBOX_RIGHT_FOREARM ,
		HITBOX_LEFT_UPPER_ARM ,
		HITBOX_LEFT_FOREARM ,
		HITBOX_MAX
	};

	struct mstudiobbox_t
	{
		int32_t m_Bone;
		int32_t m_Group; // intersection group
		Vector m_vBbmin; // bounding box 
		Vector m_vBbmax;
		int32_t m_Szhitboxnameindex; // offset to the name of the hitbox.
		int32_t pad[3];
		float m_flRadius;
		int32_t pad2[4];

		char* GetHitboxName( void )
		{
			if ( m_Szhitboxnameindex == 0 )
				return "";

			return ( ( char* )this ) + m_Szhitboxnameindex;
		}
	};

	struct mstudiohitboxset_t
	{
		int						sznameindex;
		inline char* const		GetName( void ) const { return ( ( char* )this ) + sznameindex; }
		int						numhitboxes;
		int						hitboxindex;
		inline mstudiobbox_t*	pHitbox( int i ) const { return (mstudiobbox_t*)( ( ( BYTE* )this ) + hitboxindex ) + i; };
	};

	struct mstudiobone_t
	{
		int					sznameindex;
		inline char * const GetName( void ) const { return ( ( char * )this ) + sznameindex; }
		int		 			parent;
		int					bonecontroller[6];

		Vector				pos;
		float				quat[4];
		Vector				rot;
		Vector				posscale;
		Vector				rotscale;

		matrix3x4_t			poseToBone;
		float				qAlignment[4];
		int					flags;
		int					proctype;
		int					procindex;		// procedural rule
		mutable int			physicsbone;	// index into physically simulated bone
		inline void *		GetProcedure() const { if ( procindex == 0 ) return NULL; else return  (void *)( ( ( BYTE * )this ) + procindex ); };
		int					surfacepropidx;	// index into string tablefor property name
		inline char * const GetSurfaceProps( void ) const { return ( ( char * )this ) + surfacepropidx; }
		int					contents;		// See BSPFlags.h for the contents flags

		int					unused[8];		// remove as appropriate
	};

	struct studiohdr_t
	{
		int					id;
		int					version;
		int					checksum;
		char				name[64];
		int					length;
		Vector				eyeposition;
		Vector				illumposition;
		Vector				hull_min;
		Vector				hull_max;
		Vector				view_bbmin;
		Vector				view_bbmax;
		int					flags;
		int					numbones;
		int					boneindex;

		inline mstudiobone_t *pBone( int i ) const { return (mstudiobone_t *)( ( ( BYTE * )this ) + boneindex ) + i; };

		int					numbonecontrollers;
		int					bonecontrollerindex;
		int					numhitboxsets;
		int					hitboxsetindex;

		mstudiohitboxset_t* pHitboxSet( int i ) const
		{
			return (mstudiohitboxset_t*)( ( ( BYTE* )this ) + hitboxsetindex ) + i;
		}

		inline mstudiobbox_t* pHitbox( int i , int set ) const
		{
			mstudiohitboxset_t const* s = pHitboxSet( set );

			if ( !s )
				return NULL;

			return s->pHitbox( i );
		}

		inline int GetHitboxCount( int set ) const
		{
			mstudiohitboxset_t const* s = pHitboxSet( set );

			if ( !s )
				return 0;

			return s->numhitboxes;
		}

		int					numlocalanim;
		int					localanimindex;
		int					numlocalseq;
		int					localseqindex;
		mutable int			activitylistversion;
		mutable int			eventsindexed;
		int					numtextures;
		int					textureindex;
		int					numcdtextures;
		int					cdtextureindex;
		int					numskinref;
		int					numskinfamilies;
		int					skinindex;
		int					numbodyparts;
		int					bodypartindex;
		int					numlocalattachments;
		int					localattachmentindex;
		int					numlocalnodes;
		int					localnodeindex;
		int					localnodenameindex;
		int					numflexdesc;
		int					flexdescindex;
		int					numflexcontrollers;
		int					flexcontrollerindex;
		int					numflexrules;
		int					flexruleindex;
		int					numikchains;
		int					ikchainindex;
		int					nummouths;
		int					mouthindex;
		int					numlocalposeparameters;
		int					localposeparamindex;
		int					surfacepropindex;
		int					keyvalueindex;
		int					keyvaluesize;
		int					numlocalikautoplaylocks;
		int					localikautoplaylockindex;
		float				mass;
		int					contents;
		int					numincludemodels;
		int					includemodelindex;
		mutable void		*virtualModel;
		int					szanimblocknameindex;
		int					numanimblocks;
		int					animblockindex;
		mutable void		*animblockModel;
		int					bonetablebynameindex;
		void				*pVertexBase;
		void				*pIndexBase;
		BYTE				constdirectionallightdot;
		BYTE				rootLOD;
		BYTE				numAllowedRootLODs;
		PAD( 0x5 );
		int					numflexcontrollerui;
		int					flexcontrolleruiindex;
		float				flVertAnimFixedPointScale;
		PAD( 0x4 );
		int					studiohdr2index;
		PAD( 0x4 );
	};

	enum RenderableTranslucencyType_t
	{
		RENDERABLE_IS_OPAQUE = 0 ,
		RENDERABLE_IS_TRANSLUCENT ,
		RENDERABLE_IS_TWO_PASS ,	// has both translucent and opaque sub-partsa
	};

	class IVModelInfoClient
	{
	public:
		virtual							~IVModelInfoClient( void ) { }
		virtual const model_t			*GetModel( int modelindex ) const = 0;
		// Returns index of model by name
		virtual int						GetModelIndex( const char *name ) const = 0;
		// Returns name of model
		virtual const char				*GetModelName( const model_t *model ) const = 0;
		virtual vcollide_t				*GetVCollide( const model_t *model ) const = 0;
		virtual vcollide_t				*GetVCollide( int modelindex ) const = 0;
		virtual void					GetModelBounds( const model_t *model , Vector& mins , Vector& maxs ) const = 0;
		virtual	void					GetModelRenderBounds( const model_t *model , Vector& mins , Vector& maxs ) const = 0;
		virtual int						GetModelFrameCount( const model_t *model ) const = 0;
		virtual int						GetModelType( const model_t *model ) const = 0;
		virtual void					*GetModelExtraData( const model_t *model ) = 0;
		virtual bool					ModelHasMaterialProxy( const model_t *model ) const = 0;
		virtual bool					IsTranslucent( model_t const* model ) const = 0;
		virtual bool					IsTranslucentTwoPass( const model_t *model ) const = 0;
		virtual void					Unused0() {};
		virtual RenderableTranslucencyType_t ComputeTranslucencyType( const model_t *model , int nSkin , int nBody ) = 0;
		virtual int						GetModelMaterialCount( const model_t* model ) const = 0;
		virtual void					GetModelMaterials( const model_t *model , int count , IMaterial** ppMaterials ) = 0;
		virtual bool					IsModelVertexLit( const model_t *model ) const = 0;
		virtual const char				*GetModelKeyValueText( const model_t *model ) = 0;
		virtual bool					GetModelKeyValue( const model_t *model , CUtlBuffer &buf ) = 0; // supports keyvalue blocks in submodels
		virtual float					GetModelRadius( const model_t *model ) = 0;

		virtual const studiohdr_t		*FindModel( const studiohdr_t *pStudioHdr , void **cache , const char *modelname ) const = 0;
		virtual const studiohdr_t		*FindModel( void *cache ) const = 0;
		virtual	virtualmodel_t			*GetVirtualModel( const studiohdr_t *pStudioHdr ) const = 0;
		virtual byte					*GetAnimBlock( const studiohdr_t *pStudioHdr , int iBlock ) const = 0;
		virtual bool					HasAnimBlockBeenPreloaded( studiohdr_t const* , int ) const = 0;

		// Available on client only!!!
		virtual void					GetModelMaterialColorAndLighting( const model_t *model , Vector const& origin ,
																		  QAngle const& angles , trace_t* pTrace ,
																		  Vector& lighting , Vector& matColor ) = 0;
		virtual void					GetIlluminationPoint( const model_t *model , IClientRenderable *pRenderable , Vector const& origin ,
															  QAngle const& angles , Vector* pLightingCenter ) = 0;

		virtual int  					GetModelContents(int modelIndex) const = 0;
		virtual void					UNUSED() = 0;
		virtual void 					UNUSE11D() = 0;
		virtual studiohdr_t  			*GetStudioModel(const model_t *mod) = 0;
		virtual int   					GetModelSpriteWidth(const model_t *model) const = 0;
		virtual int   					GetModelSpriteHeight(const model_t *model) const = 0;

		// Sets/gets a map-specified fade range (client only)
		virtual void					SetLevelScreenFadeRange( float flMinSize , float flMaxSize ) = 0;
		virtual void					GetLevelScreenFadeRange( float *pMinArea , float *pMaxArea ) const = 0;

		// Sets/gets a map-specified per-view fade range (client only)
		virtual void					SetViewScreenFadeRange( float flMinSize , float flMaxSize ) = 0;

		// Computes fade alpha based on distance fade + screen fade (client only)
		virtual unsigned char			ComputeLevelScreenFade( const Vector &vecAbsOrigin , float flRadius , float flFadeScale ) const = 0;
		virtual unsigned char			ComputeViewScreenFade( const Vector &vecAbsOrigin , float flRadius , float flFadeScale ) const = 0;

		// both client and server
		virtual int						GetAutoplayList( const studiohdr_t *pStudioHdr , unsigned short **pAutoplayList ) const = 0;

		// Gets a virtual terrain collision model (creates if necessary)
		// NOTE: This may return NULL if the terrain model cannot be virtualized
		virtual CPhysCollide			*GetCollideForVirtualTerrain( int index ) = 0;
		virtual bool					IsUsingFBTexture( const model_t *model , int nSkin , int nBody , void /*IClientRenderable*/ *pClientRenderable ) const = 0;
		virtual const model_t			*FindOrLoadModel( const char *name ) const = 0;
		virtual MDLHandle_t				GetCacheHandle( const model_t *model ) const = 0;
		// Returns planes of non-nodraw brush model surfaces
		virtual int						GetBrushModelPlaneCount( const model_t *model ) const = 0;
		virtual void					GetBrushModelPlane( const model_t *model , int nIndex , cplane_t &plane , Vector *pOrigin ) const = 0;
		virtual int						GetSurfacepropsForVirtualTerrain( int index ) = 0;
		virtual bool					UsesEnvCubemap( const model_t *model ) const = 0;
		virtual bool					UsesStaticLighting( const model_t *model ) const = 0;
	};
}