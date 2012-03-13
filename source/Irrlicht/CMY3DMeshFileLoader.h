// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// This file was originally written by ZDimitor.
// I (Nikolaus Gebhardt) did some few changes to this:
// - replaced logging calls to their os:: counterparts
// - removed some logging calls
// - removed setTexture path and replaced it with the directory of the mesh
// - added EAMT_MY3D file type
// - fixed a memory leak when decompressing RLE data.
// - cleaned multi character constant problems with gcc
// - removed octree child scene node generation because irrlicht is now able to draw
//   scene nodes with transparent and sold materials in them at the same time. (see changes.txt)
// Thanks a lot to ZDimitor for his work on this and that he gave me
// his permission to add it into Irrlicht.

//--------------------------------------------------------------------------------
// This tool created by ZDimitor everyone can use it as wants
//--------------------------------------------------------------------------------

#ifndef __CMY3D_MESH_FILE_LOADER_H_INCLUDED__
#define __CMY3D_MESH_FILE_LOADER_H_INCLUDED__


#ifdef _MSC_VER
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif


#include "IMeshLoader.h"
#include "SMesh.h"
#include "CMeshBuffer.h"
#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "irrString.h"
#include "ISceneManager.h"

namespace irr
{
namespace scene
{

//--------------------------------------------------------------------
// byte-align structures
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#   pragma pack( push, packing )
#   pragma pack( 1 )
#   define PACK_STRUCT
#elif defined( __GNUC__ )
#   define PACK_STRUCT  __attribute__((packed))
#else
#   error compiler not supported
#endif
//----------------------------------------------------------------------
struct SMyColor
{   SMyColor () {;}
    SMyColor (s32 __R, s32 __G, s32 __B, s32 __A)
        : R(__R), G(__G), B(__B), A(__A) {}
    s32 R, G, B, A;
} PACK_STRUCT;

// material header
struct SMyMaterialHeader
{   c8  Name[256];           // material name
    u32 Index;
    SMyColor AmbientColor;
    SMyColor DiffuseColor;
    SMyColor EmissiveColor;
    SMyColor SpecularColor;
    f32 Shininess;
    f32 Transparency;
    u32 TextureCount;        // texture count
} PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#   pragma pack( pop, packing )
#endif

class CMY3DMeshFileLoader : public IMeshLoader
{
public:
	CMY3DMeshFileLoader(ISceneManager *scmgr, io::IFileSystem* fs);
	virtual ~CMY3DMeshFileLoader();

	virtual bool isALoadableFileExtension(const io::path& filename) const;

	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

	//! getting access to the nodes (with transparent material), creating
	//! while loading .my3d file
	const core::array<ISceneNode*>& getChildNodes() const;

private:

	video::ITexture* readEmbeddedLightmap(io::IReadFile* file, char* namebuf);

	scene::ISceneManager* SceneManager;
	io::IFileSystem* FileSystem;

	struct SMyMaterialEntry
	{
		SMyMaterialEntry ()
		: Texture1FileName("null"), Texture2FileName("null"),
		Texture1(0), Texture2(0), MaterialType(video::EMT_SOLID) {}

		SMyMaterialHeader Header;
		core::stringc Texture1FileName;
		core::stringc Texture2FileName;
		video::ITexture *Texture1;
		video::ITexture *Texture2;
		video::E_MATERIAL_TYPE MaterialType;
	};

	struct SMyMeshBufferEntry
	{
		SMyMeshBufferEntry() : MaterialIndex(-1), MeshBuffer(0) {}
		SMyMeshBufferEntry(s32 mi, CMeshBuffer<video::S3DVertex2TCoords>* mb)
			: MaterialIndex(mi), MeshBuffer(mb) {}

		s32 MaterialIndex;
		CMeshBuffer<video::S3DVertex2TCoords>* MeshBuffer;
	};

	SMyMaterialEntry*    getMaterialEntryByIndex     (u32 matInd);
	CMeshBuffer<video::S3DVertex2TCoords>* getMeshBufferByMaterialIndex(u32 matInd);

	core::array<SMyMaterialEntry>   MaterialEntry;
	core::array<SMyMeshBufferEntry> MeshBufferEntry;

	core::array<ISceneNode*> ChildNodes;
};


} // end namespace scene
} // end namespace irr


#endif // __CMY3D_MESH_FILE_LOADER_H_INCLUDED__
