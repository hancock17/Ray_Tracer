#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "camera.h"
#include "light.h"
#include "material.h"
#include "shape.h"

class Scene {
public:
    Scene() : backgroundColor(0, 0, 0), shadowRayEpsilon(0.001f), maxRecursionDepth(0) {}
    ~Scene();

    void loadFromFile(const std::string& filename);
    Vector3f computeColor(const Ray& ray, int depth);
    bool findClosestHit(const Ray& ray, HitRecord& best_rec, Shape*& closest_obj);

    Vector3f backgroundColor;
    float shadowRayEpsilon;
    int maxRecursionDepth;

    std::vector<Camera> cameras;
    AmbientLight ambientLight;
    std::vector<PointLight> pointLights;
    std::vector<Material> materials;
    std::vector<Vector3f> vertices;
    std::vector<Shape*> objects;

private:
    Material* findMaterialById(int materialId);
};

#endif
