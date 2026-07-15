#ifndef SHAPE_H
#define SHAPE_H

#include "vector3f.h"
#include "ray.h"

struct HitRecord {
    float t; // Hit distance
    Vector3f normal; // Surface normal
    int materialId; // Material id
};

class Shape {
public:
    int id; // Object id
    int materialId; // Material id

    virtual bool intersect(const Ray& ray, HitRecord& rec) const = 0;
    virtual ~Shape() {}
};

#endif
