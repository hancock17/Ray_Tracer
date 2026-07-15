#ifndef CAMERA_H
#define CAMERA_H

#include "vector3f.h"
#include "ray.h"

class Camera {
public:
    Vector3f position;
    Vector3f gaze;
    Vector3f up;

    float left, right, bottom, top;
    float distance;
    int width, height;

    Vector3f u, v, w;

    void computeBasis() {
        // Builds camera basis vectors
        w = -gaze.normalized();
        u = up.cross(w).normalized();
        v = w.cross(u);
    }

    Ray generateRay(int i, int j) const {
        // Samples the image plane at pixel center
        float u_coord = left + (right - left) * (i + 0.5f) / width;
        float v_coord = top - (top - bottom) * (j + 0.5f) / height;

        Vector3f s = position + (u * u_coord) + (v * v_coord) - (w * distance);
        return Ray(position, (s - position).normalized());
    }
};

#endif
