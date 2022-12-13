#include "../headers/cringetracer/Materials/MtlLib.h"

MtlLib::MtlLib()
{
}

MtlLib::~MtlLib()
{
}

bool MtlLib::Lookup(const char* name, Material*& outMtl)
{
	/*auto mtlFound = std::find_if(materials.begin(), materials.end(), [&](const Material* mtl)
        {
            return mtl->Name() == name;
        });
    if (mtlFound == materials.end()) return false;
    outMtl = *mtlFound;*/
    return true;
}
