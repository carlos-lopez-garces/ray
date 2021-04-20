This repository is the result of studying the Ray Tracing in One Weekend series, Peter Shirley ([online edition](https://raytracing.github.io/)).

The sections I've covered are marked next. My personal notes are found in **[notes](notes/)**.

1 Overview

2 Output an Image
- [x] 2.1 The PPM Image Format
- [x] 2.2 Creating an Image File
- [x] 2.3 Adding a Progress Indicator

3 The vec3 Class
- [x] 3.1 Variables and Methods
- [x] 3.2 vec3 Utility Functions
- [x] 3.3 Color Utility Functions

4 Rays, a Simple Camera, and Background
- [x] 4.1 The ray Class
- [x] 4.2 Sending Rays Into the Scene

5 Adding a Sphere
- [x] 5.1 Ray-Sphere Intersection
- [x] 5.2 Creating Our First Raytraced Image

6 Surface Normals and Multiple Objects
- [x] 6.1 Shading with Surface Normals
- [x] 6.2 Simplifying the Ray-Sphere Intersection Code
- [x] 6.3 An Abstraction for Hittable Objects
- [x] 6.4 Front Faces Versus Back Faces
- [x] 6.5 A List of Hittable Objects
- [x] 6.6 Some New C++ Features
- [x] 6.7 Common Constants and Utility Functions

7 Antialiasing
- [x] 7.1 Some Random Number Utilities
- [x] 7.2 Generating Pixels with Multiple Samples

8 Diffuse Materials
- [x] 8.1 A Simple Diffuse Material
- [x] 8.2 Limiting the Number of Child Rays
- [x] 8.3 Using Gamma Correction for Accurate Color Intensity
- [x] 8.4 Fixing Shadow Acne
- [x] 8.5 True Lambertian Reflection
- [x] 8.6 An Alternative Diffuse Formulation

9 Metal
- [x] 9.1 An Abstract Class for Materials
- [x] 9.2 A Data Structure to Describe Ray-Object Intersections
- [x] 9.3 Modeling Light Scatter and Reflectance
- [x] 9.4 Mirrored Light Reflection
- [x] 9.5 A Scene with Metal Spheres
- [x] 9.6 Fuzzy Reflection

10 Dielectrics
- [x] 10.1 Refraction
- [x] 10.2 Snell's Law
- [x] 10.3 Total Internal Reflection
- [x] 10.4 Schlick Approximation
- [x] 10.5 Modeling a Hollow Glass Sphere

11 Positionable Camera
- [x] 11.1 Camera Viewing Geometry
- [x] 11.2 Positioning and Orienting the Camera

12 Defocus Blur
- [x] 12.1 A Thin Lens Approximation
- [x] 12.2 Generating Sample Rays

13 Where Next?
- [x] 13.1 A Final Render
- [x] 13.2 Next Steps