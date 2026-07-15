#ifndef RAY_H
#define RAY_H

#include "vector3f.h"

class Ray {
public:
    Vector3f origin; // Ray origin
    Vector3f direction; // Ray direction

    Ray(const Vector3f& o, const Vector3f& d) : origin(o), direction(d) {}

    // Computes point on ray
    Vector3f pointAt(float t) const {
        return origin + t * direction;
    }
};

#endif
