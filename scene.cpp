#include "scene.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "mesh.h"
#include "sphere.h"
#include "triangle.h"

namespace {
float manual_max(float a, float b) {
    return (a > b) ? a : b;
}
}

Scene::~Scene() {
    // Releases heap-allocated shapes
    for (size_t i = 0; i < objects.size(); ++i)
        delete objects[i];
}

void Scene::loadFromFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    std::string tag;

    if (!file.is_open()) {
        std::cerr << "Could not open input file!" << std::endl;
        return;
    }

    while (file >> tag) {
        // For parsing: reads tags and populates scene data
        if (tag == "#BackgroundColor")
            file >> backgroundColor.x >> backgroundColor.y >> backgroundColor.z;
        else if (tag == "#MaxRecursionDepth")
            file >> maxRecursionDepth;
        else if (tag == "#ShadowRayEpsilon")
            file >> shadowRayEpsilon;
        else if (tag == "#Camera") {
            Camera cam;
            file >> cam.position.x >> cam.position.y >> cam.position.z;
            file >> cam.gaze.x >> cam.gaze.y >> cam.gaze.z;
            file >> cam.up.x >> cam.up.y >> cam.up.z;
            file >> cam.left >> cam.right >> cam.bottom >> cam.top;
            file >> cam.distance;
            file >> cam.width >> cam.height;
            cameras.push_back(cam);
        }
        else if (tag == "#Material") {
            Material mat;
            file >> mat.id;
            file >> mat.ambient.x >> mat.ambient.y >> mat.ambient.z;
            file >> mat.diffuse.x >> mat.diffuse.y >> mat.diffuse.z;
            file >> mat.specular.x >> mat.specular.y >> mat.specular.z;
            file >> mat.phong_exponent;
            file >> mat.mirror.x >> mat.mirror.y >> mat.mirror.z;
            materials.push_back(mat);
        }
        else if (tag == "#AmbientLight") {
            file >> ambientLight.intensity.x >> ambientLight.intensity.y >> ambientLight.intensity.z;
        }
        else if (tag == "#PointLight") {
            PointLight pl;
            file >> pl.id;
            file >> pl.position.x >> pl.position.y >> pl.position.z;
            file >> pl.intensity.x >> pl.intensity.y >> pl.intensity.z;
            pointLights.push_back(pl);
        }
        else if (tag == "#VertexList") {
            // Read vertices until the next tag
            while (!(file >> std::ws).eof() && file.peek() != '#') {
                float x, y, z;
                if (!(file >> x >> y >> z)) break;
                vertices.push_back(Vector3f(x, y, z));
            }
        }
        else if (tag == "#Sphere") {
            int id, matId, centerInd;
            float radius;
            file >> id >> matId >> centerInd >> radius;
            if (centerInd > 0 && centerInd <= static_cast<int>(vertices.size()))
                objects.push_back(new Sphere(id, matId, vertices[centerInd - 1], radius));
        }
        else if (tag == "#Triangle") {
            int id, matId, v0, v1, v2;
            file >> id >> matId >> v0 >> v1 >> v2;
            if (v0 > 0 && v0 <= static_cast<int>(vertices.size()) &&
                v1 > 0 && v1 <= static_cast<int>(vertices.size()) &&
                v2 > 0 && v2 <= static_cast<int>(vertices.size())) {
                objects.push_back(new Triangle(id, matId, vertices[v0 - 1], vertices[v1 - 1], vertices[v2 - 1]));
            }
        }
        else if (tag == "#Mesh") {
            int id, matId;
            file >> id >> matId;

            std::vector<Triangle> meshTriangles;
            while (!(file >> std::ws).eof() && file.peek() != '#') {
                int v0, v1, v2;
                if (!(file >> v0 >> v1 >> v2))  break;
                if (v0 > 0 && v0 <= static_cast<int>(vertices.size()) &&
                    v1 > 0 && v1 <= static_cast<int>(vertices.size()) &&
                    v2 > 0 && v2 <= static_cast<int>(vertices.size())) {
                    meshTriangles.push_back(Triangle(id, matId, vertices[v0 - 1], vertices[v1 - 1], vertices[v2 - 1]));
                }
            }
            objects.push_back(new Mesh(id, matId, meshTriangles));
        }
    }
}

Material* Scene::findMaterialById(int materialId) {
    for (size_t i = 0; i < materials.size(); ++i) {
        if (materials[i].id == materialId)
            return &materials[i];
    }
    return nullptr;
}

bool Scene::findClosestHit(const Ray& ray, HitRecord& best_rec, Shape*& closest_obj) {
    float min_t = 1e30f;
    closest_obj = nullptr;

    for (size_t i = 0; i < objects.size(); ++i) {
        HitRecord temp_rec;
        if (objects[i]->intersect(ray, temp_rec) && temp_rec.t > shadowRayEpsilon && temp_rec.t < min_t) {
            min_t = temp_rec.t;
            best_rec = temp_rec;
            closest_obj = objects[i];
        }
    }
    return closest_obj != nullptr;
}

Vector3f Scene::computeColor(const Ray& ray, int depth) {
    // Core Shading function: Ambient + Diffuse + Specular + Mirror Reflection
    if (depth > maxRecursionDepth)
        return Vector3f(0, 0, 0);

    Shape* closest_obj = nullptr;
    HitRecord best_rec;

    findClosestHit(ray, best_rec, closest_obj);

    if (!closest_obj)
        return backgroundColor;

    Material* mat = findMaterialById(best_rec.materialId);
    if (!mat)
        return backgroundColor;

    Vector3f hitPoint = ray.pointAt(best_rec.t);
    Vector3f N = best_rec.normal.normalized();
    Vector3f V = (-ray.direction).normalized();
    Vector3f color = mat->ambient.mult(ambientLight.intensity);

    for (size_t i = 0; i < pointLights.size(); ++i) {
        const PointLight& light = pointLights[i];
        Vector3f lightVec = light.position - hitPoint;
        float distSq = lightVec.dot(lightVec);
        float distToLight = std::sqrt(distSq);

        if (distSq <= 0.0f)
            continue;

        Vector3f L = lightVec.normalized();

        // determines if light source is blocked to check shadow ray
        Ray shadowRay(hitPoint + N * shadowRayEpsilon, L);
        bool inShadow = false;

        for (size_t j = 0; j < objects.size(); ++j) {
            HitRecord shadowRec;

            if (objects[j]->intersect(shadowRay, shadowRec) &&
                shadowRec.t > shadowRayEpsilon &&
                shadowRec.t < distToLight) {
                inShadow = true;
                break;
            }
        }

        if (inShadow)   continue;

        // Applies inverse-square falloff
        Vector3f intensityAtPoint = light.intensity * (1.0f / distSq);
        float cosTheta = manual_max(0.0f, N.dot(L));
        color = color + mat->diffuse.mult(intensityAtPoint) * cosTheta;

        // Uses Blinn-Phong for highlights
        Vector3f H = (L + V).normalized();
        float cosAlpha = manual_max(0.0f, N.dot(H));
        float phongTerm = std::pow(cosAlpha, mat->phong_exponent);
        color = color + mat->specular.mult(intensityAtPoint) * phongTerm;
    }

    if (mat->mirror.x > 0.0f || mat->mirror.y > 0.0f || mat->mirror.z > 0.0f) {
        // Trace ideal reflection ray
        float dDotN = ray.direction.dot(N);
        Vector3f R = (ray.direction - N * (2.0f * dDotN)).normalized();
        Ray reflectRay(hitPoint + N * shadowRayEpsilon, R);
        Vector3f reflectedColor = computeColor(reflectRay, depth + 1);
        color = color + mat->mirror.mult(reflectedColor);
    }

    return color;
}
