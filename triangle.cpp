#include "triangle.h"

bool Triangle::intersect(const Ray& ray, HitRecord& rec) const {
    // Based on the Cramer's Rule derivation from lecture notes
    float a = v0.x - v1.x;
    float b = v0.y - v1.y;
    float c = v0.z - v1.z;

    float d = v0.x - v2.x;
    float e = v0.y - v2.y;
    float f = v0.z - v2.z;

    float g = ray.direction.x;
    float h = ray.direction.y;
    float i = ray.direction.z;

    float j = v0.x - ray.origin.x;
    float k = v0.y - ray.origin.y;
    float l = v0.z - ray.origin.z;

    float ei_hf = e * i - h * f;
    float gf_di = g * f - d * i;
    float dh_eg = d * h - e * g;

    float detA = a * ei_hf + b * gf_di + c * dh_eg;
    if (detA > -0.000001f && detA < 0.000001f) return false;

    float invDetA = 1.0f / detA;

    float ak_jb = a * k - j * b;
    float jc_al = j * c - a * l;
    float bl_kc = b * l - k * c;

    float beta = (j * ei_hf + k * gf_di + l * dh_eg) * invDetA;
    if (beta < 0.0f || beta > 1.0f) return false;

    float gamma = (i * ak_jb + h * jc_al + g * bl_kc) * invDetA;
    if (gamma < 0.0f || (beta + gamma) > 1.0f) return false;

    float t = -(f * ak_jb + e * jc_al + d * bl_kc) * invDetA;
    if (t <= 0.000001f) return false;

    // Stores hit data
    rec.t = t;
    rec.materialId = this->materialId;
    rec.normal = (v1 - v0).cross(v2 - v0).normalized();

    return true;
}
