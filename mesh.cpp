#include "mesh.h"

bool Mesh::intersect(const Ray& ray, HitRecord& rec) const {
    float min_t = 1e30f;
    bool hit = false;
    HitRecord temp_rec;
    for (const auto& tri : faces) {
        // Tests all mesh faces
        if (tri.intersect(ray, temp_rec)) {
            if (temp_rec.t < min_t && temp_rec.t > 0) {
                min_t = temp_rec.t;
                rec = temp_rec;
                hit = true;
            }
        }
    }
    if (hit)
        rec.materialId = this->materialId;

    return hit;
}
