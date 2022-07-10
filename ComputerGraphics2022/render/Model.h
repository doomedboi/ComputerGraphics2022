#pragma once
#include "Mesh.h"
#include <vector>
#include <string>

struct CRawModel {
    std::vector<CMesh*> meshes;
    std::vector<std::string> materialNames;
};