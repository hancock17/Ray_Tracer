#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"

class Sphere : public Shape {
public:
    Vector3f center;
    float radius;

    Sphere(int id, int mat_id, Vector3f c, float r) {
        this->id = id;
        this->materialId = mat_id;
        this->center = c;
        this->radius = r;
    }

    virtual bool intersect(const Ray& ray, HitRecord& rec) const override;
};
#endif
