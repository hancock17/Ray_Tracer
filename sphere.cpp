#include "sphere.h"
#include <cmath>

bool Sphere::intersect(const Ray& ray, HitRecord& rec) const {
    Vector3f emc = ray.origin - this->center;

    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * ray.direction.dot(emc);
    float c = emc.dot(emc) - (radius * radius);

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;

    float sqrt_d = sqrt(discriminant);
    float t1 = (-b - sqrt_d) / (2.0f * a);
    float t2 = (-b + sqrt_d) / (2.0f * a);

    float t = -1.0f;
    if (t1 > 0 && t2 > 0) t = (t1 < t2) ? t1 : t2;
    else if (t1 > 0) t = t1;
    else if (t2 > 0) t = t2;

    if (t <= 0) return false;

    // Records the hit information for shading calculations
    rec.t = t;
    Vector3f hitPoint = ray.pointAt(t);
    rec.normal = (hitPoint - center).normalized();
    rec.materialId = this->materialId;

    return true;
}
