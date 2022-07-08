#pragma once
#include "Mesh.h"
#include <vector>

struct CRawModel {
    std::vector<CMesh*> meshes;
    std::vector<std::string> materialNames;
};