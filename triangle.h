#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape {
public:
    Vector3f v0, v1, v2;

    Triangle(int id, int mat_id, Vector3f p0, Vector3f p1, Vector3f p2) {
        this->id = id;
        this->materialId = mat_id;
        this->v0 = p0; this->v1 = p1; this->v2 = p2;
    }

    virtual bool intersect(const Ray& ray, HitRecord& rec) const override;
};
#endif
