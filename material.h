#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector3f.h"

struct Material {
    int id;
    Vector3f ambient; // Ambient color
    Vector3f diffuse; // Diffuse color
    Vector3f specular; // Specular color
    Vector3f mirror; // Mirror color
    float phong_exponent; // Phong exponent
};

#endif
