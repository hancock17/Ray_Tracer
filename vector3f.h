#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <cmath>

struct Vector3f {
    float x, y, z;
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3f operator+(const Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
    Vector3f operator-(const Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }
    Vector3f operator*(float s) const { return Vector3f(x * s, y * s, z * s); }
    Vector3f operator-() const { return Vector3f(-x, -y, -z); }

    float dot(const Vector3f& v) const { return x * v.x + y * v.y + z * v.z; }
    Vector3f cross(const Vector3f& v) const {
        return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3f normalized() const { float n = norm(); return (n > 0) ? (*this * (1.0f / n)) : *this; }
    Vector3f mult(const Vector3f& v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }
};

inline Vector3f operator*(float s, const Vector3f& v) { return v * s; }
#endif
