#ifndef MESH_H
#define MESH_H

#include <vector>
#include "triangle.h"

class Mesh : public Shape {
public:
    std::vector<Triangle> faces;
    Mesh(int id, int mat_id, const std::vector<Triangle>& tris) {
        this->id = id;
        this->materialId = mat_id;
        this->faces = tris;
    }
    virtual bool intersect(const Ray& ray, HitRecord& rec) const override;
};
#endif
