#ifndef LIGHT_H
#define LIGHT_H

#include "vector3f.h"

struct AmbientLight {
    Vector3f intensity; // Ambient intensity

    AmbientLight() : intensity(0, 0, 0) {}
};

struct PointLight {
    int id;
    Vector3f position; // Light position
    Vector3f intensity; // Light intensity

    PointLight() : id(0), position(0, 0, 0), intensity(0, 0, 0) {}
};

#endif
