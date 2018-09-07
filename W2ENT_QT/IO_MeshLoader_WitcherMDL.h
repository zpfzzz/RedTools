#ifndef CWITCHERMDLMESHFILELOADER_H
#define CWITCHERMDLMESHFILELOADER_H

#include "IMeshLoader.h"
#include "irrString.h"
#include "SMesh.h"
#include "SAnimatedMesh.h"
#include "IMeshManipulator.h"
#include "ISkinnedMesh.h"
#include "IFileSystem.h"
#include "IReadFile.h"
#include "Log.h"
#include "IO_SpeedTreeLoader.h"

#include <vector>
#include <map>

// Based on the loader and spec of the Xoreos engine
// https://github.com/xoreos/xoreos/blob/master/src/graphics/aurora/model_witcher.cpp

using namespace irr;

struct ArrayDef
{
    u32 firstElemOffest;
    u32 nbUsedEntries;
    u32 nbAllocatedEntries;
};

struct ModelData
{
    u16 fileVersion;

    u32 offsetModelData;
    u32 sizeModelData;

    u32 offsetRawData;
    u32 sizeRawData;

    u32 offsetTextureInfo;

    u32 offsetTexData;
    u32 sizeTexData;
};

struct ControllersData
{
    // transform
    core::vector3df position;
    core::quaternion rotation;
    core::vector3df scale;
    core::matrix4 localTransform;
    core::matrix4 globalTransform;

    // others
    f32 alpha;
    video::SColor selphIllumColor;

    ControllersData() : alpha(1.f) {}
};

struct AnimationControllersData
{
    core::array<f32> positionTime;
    core::array<core::vector3df> position;

    core::array<f32> rotationTime;
    core::array<core::quaternion> rotation;

    core::array<f32> scaleTime;
    core::array<core::vector3df> scale;
};


class TW1_MaterialParser
{
public :
    TW1_MaterialParser(io::IFileSystem *fs);
    bool loadFile(core::stringc filename);
    bool loadFromString(core::stringc content);
    bool hasMaterial();
    void debugPrint();

    core::stringc getShader();
    core::stringc getTexture(u32 slot);
    video::E_MATERIAL_TYPE getMaterialTypeFromShader();

private:
    Log* _log;
    io::IFileSystem* FileSystem;
    core::stringc _shader;
    std::map<core::stringc, core::stringc> _textures;
    std::map<core::stringc, core::stringc> _bumpmaps;
    std::map<core::stringc, core::stringc> _strings;
    // TODO: vector4 ?
    std::map<core::stringc, core::vector3df> _vectors;
    std::map<core::stringc, f32> _floats;
};

struct SkinMeshToLoadEntry
{
    long Seek;
    ControllersData Controller;
};

class IO_MeshLoader_WitcherMDL : public scene::IMeshLoader
{
public:
    IO_MeshLoader_WitcherMDL(scene::ISceneManager *smgr, io::IFileSystem *fs);

    //! returns true if the file maybe is able to be loaded by this class
    //! based on the file extension (e.g. ".cob")
    virtual bool isALoadableFileExtension(const io::path& filename) const;

    //! creates/loads an animated mesh from the file.
    //! \return Pointer to the created mesh. Returns 0 if loading failed.
    //! If you no longer need the mesh, you should call IAnimatedMesh::drop().
    //! See IReferenceCounted::drop() for more information.
    virtual scene::IAnimatedMesh* createMesh(io::IReadFile* file);

private:
    std::map<u32, core::stringc> NodeTypeNames;

    bool load(io::IReadFile* file);
    void loadNode(io::IReadFile* file, scene::ISkinnedMesh::SJoint *parentJoint, core::matrix4 parentMatrix);
    void readMeshNode(io::IReadFile* file, ControllersData controllers);
    void readTexturePaintNode(io::IReadFile* file, ControllersData controllers);
    void readSkinNode(io::IReadFile* file, ControllersData controllers);
    void readSpeedtreeNode(io::IReadFile* file, ControllersData controllers);
    TW1_MaterialParser readTextures(io::IReadFile *file);

    // Animations
    void readAnimations(io::IReadFile* file);
    void loadAnimationNode(io::IReadFile* file, f32 timeOffset);
    AnimationControllersData readNodeAnimControllers(io::IReadFile* file, ArrayDef key, ArrayDef data);

    template <class T> core::array<T> readArray(io::IReadFile* file, ArrayDef def);

    ControllersData readNodeControllers(io::IReadFile* file, ArrayDef key, ArrayDef data);
    void transformVertices(core::matrix4 mat);

    bool hasTexture(core::stringc texPath);
    video::ITexture* getTexture(core::stringc texPath);


    scene::ISceneManager* SceneManager;
    io::IFileSystem* FileSystem;
    scene::ISkinnedMesh* AnimatedMesh;

    ModelData ModelInfos;
    
    core::stringc GameTexturesPath;

    core::array<SkinMeshToLoadEntry> SkinMeshToLoad;

    Log* _log;
    u32 _depth;
};

#endif // CWITCHERMDLMESHFILELOADER_H
