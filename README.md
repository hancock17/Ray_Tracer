# Ray Tracing Renderer

A small CPU-based ray tracer written in C++. The renderer reads a text scene description and produces a binary PPM (`P6`) image for every camera in the scene.

This project was developed as part of a Computer Graphics course assignment.

## Features

- Perspective cameras with pixel-centre sampling
- Sphere, triangle, and triangle-mesh geometry
- Closest-hit ray intersections
- Ambient, diffuse, and Blinn-Phong specular lighting
- Point lights with inverse-square attenuation
- Hard shadows using shadow rays
- Ideal mirror reflections with configurable recursion depth
- Plain-text scene descriptions and dependency-free C++ source code

## Requirements

- A C++ compiler with C++11 support or later (GCC, MinGW, Clang, or MSVC)
- Optional: [Code::Blocks](https://www.codeblocks.org/) to open `ray_tracing.cbp`

No external libraries are required.

## Build

From the project directory, compile the sources with:

```bash
g++ -std=c++11 -O2 -Wall main.cpp scene.cpp sphere.cpp triangle.cpp mesh.cpp -o ray_tracing
```

On Windows with MinGW, the executable will be `ray_tracing.exe`.

Alternatively, open `ray_tracing.cbp` in Code::Blocks and build the **Debug** or **Release** target.

## Run

Run the renderer with a scene file:

```bash
./ray_tracing input1.txt
```

If no file is supplied, the program uses `input1.txt` by default.

The output is written next to the executable's working directory using the input file name. For example:

```text
input1.txt  ->  input1.ppm
input_ex1.txt  ->  input_ex1.ppm
```

For scenes containing multiple cameras, additional images use the suffix `_2`, `_3`, and so on. PPM images can be opened by many image viewers or converted to PNG with tools such as ImageMagick.

## Example Scenes

The repository includes several sample scene files:

- `input1.txt`, `input2.txt`, and `input3.txt`
- `input_ex1.txt` and `input_ex2.txt`

Render any of them by passing its path as the first command-line argument.

## Scene File Format

Scenes are organized as tagged sections. Indices in geometry definitions are **1-based** and refer to entries in `#VertexList`.

```text
#BackgroundColor
R G B

#MaxRecursionDepth
depth

#ShadowRayEpsilon
epsilon

#Camera
position_x position_y position_z
gaze_x gaze_y gaze_z
up_x up_y up_z
left right bottom top
near_distance
image_width image_height

#Material
material_id
ambient_r ambient_g ambient_b
diffuse_r diffuse_g diffuse_b
specular_r specular_g specular_b
phong_exponent
mirror_r mirror_g mirror_b

#AmbientLight
R G B

#PointLight
light_id
position_x position_y position_z
intensity_r intensity_g intensity_b

#VertexList
x y z
...

#Sphere
object_id material_id center_vertex_index radius

#Triangle
object_id material_id vertex_1 vertex_2 vertex_3

#Mesh
object_id material_id
vertex_1 vertex_2 vertex_3
...
```

Each section continues until the next line beginning with `#`. See `input1.txt` for a complete example.

## Project Structure

```text
main.cpp                 Program entry point and PPM writer
scene.*                  Scene parsing, lighting, shadows, and reflections
camera.h                 Camera basis and primary-ray generation
ray.h / vector3f.h       Ray and vector math utilities
shape.h                  Common geometry interface
sphere.*                 Sphere-ray intersections
triangle.*               Triangle-ray intersections
mesh.*                   Mesh intersection using triangle faces
input*.txt               Example scene descriptions
ray_tracing.cbp          Code::Blocks project file
```

## Notes and Limitations

- The renderer uses brute-force intersection testing; meshes and scenes do not use acceleration structures such as BVHs.
- Output colours are clamped to the `[0, 255]` range before writing.
- The renderer currently supports hard shadows, ideal reflections, point lights, and the primitives listed above.

## License

No license has been specified for this repository yet. Add a license file before publishing if you want to define how others may use the code.
