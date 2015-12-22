#ifndef CSKELETON_H
#define CSKELETON_H

#include <irrlicht.h>

using namespace irr;

scene::ISkinnedMesh::SJoint* getJointByName(scene::ISkinnedMesh* mesh, core::stringc name);

class CSkeleton
{
public:
    CSkeleton();

    u32 nbBones;
    core::array<core::stringc> names;
    core::array<short> parentId;
    core::array<core::matrix4> matrix;

    bool applyToModel(scene::ISkinnedMesh* mesh);
    bool checkIfPerfectlyCorresponding(scene::ISkinnedMesh* mesh);
};

#endif // CSKELETON_H
