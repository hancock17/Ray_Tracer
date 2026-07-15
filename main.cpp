#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "scene.h"

std::string getBaseNameWithoutExtension(const std::string& path) {
    // Extracts file name without extension
    size_t slashPos = path.find_last_of("/\\");
    size_t start = (slashPos == std::string::npos) ? 0 : (slashPos + 1);
    size_t dotPos = path.find_last_of('.');

    if (dotPos == std::string::npos || dotPos < start)
        return path.substr(start);

    return path.substr(start, dotPos - start);
}

void saveAsPPM(const std::vector<unsigned char>& image, int width, int height, const std::string& filename) {
    // Writes the pixel data to a PPM file
    std::ofstream out(filename.c_str(), std::ios::out | std::ios::binary);
    if (!out) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    out << "P6\n" << width << " " << height << "\n255\n";
    out.write(reinterpret_cast<const char*>(image.data()), image.size());
}

unsigned char colorToByte(float val) {
    // Clamps float color and rounds to nearest integer
    if (val < 0.0f) val = 0.0f;
    if (val > 255.0f)   val = 255.0f;
    return static_cast<unsigned char>(val + 0.5f);
}

int main(int argc, char* argv[]) {
    std::string inputFile = "input1.txt";  // should be change for tests
    if (argc > 1)   inputFile = argv[1];

    Scene scene;
    scene.loadFromFile(inputFile);

    for (size_t c = 0; c < scene.cameras.size(); ++c) {
        // Process each camera defined in the scene
        Camera& cam = scene.cameras[c];
        cam.computeBasis();

        std::vector<unsigned char> pixelData;
        pixelData.reserve(cam.width * cam.height * 3);

        std::cout << "Rendering Camera " << (c + 1) << " (" << cam.width << "x" << cam.height << ")..." << std::endl;

        for (int j = 0; j < cam.height; ++j) {
                // For the image plane
            for (int i = 0; i < cam.width; ++i) {
                // Generates primary ray from camera through the pixel
                Ray ray = cam.generateRay(i, j);
                Vector3f color = scene.computeColor(ray, 0);

                pixelData.push_back(colorToByte(color.x));
                pixelData.push_back(colorToByte(color.y));
                pixelData.push_back(colorToByte(color.z));
            }
        }

        std::string baseName = getBaseNameWithoutExtension(inputFile);
        std::string outName = (c == 0) ? (baseName + ".ppm") : (baseName + "_" + std::to_string(c + 1) + ".ppm");
        saveAsPPM(pixelData, cam.width, cam.height, outName);
        std::cout << outName << " saved." << std::endl;
    }

    return 0;
}
