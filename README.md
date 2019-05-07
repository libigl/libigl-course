# libigl-course
Material for the short course on libigl. The course is composed of two parts. An overview of the coding style and purposed of the library (available in the [pptx slides](https://github.com/libigl/libigl-course/raw/master/libigl-course-slides.pptx.zip) in this repo), plus a live tutorial session. A longer version of the tutorial, which also contains an example of a vector field interpolation algorithm implemented in libigl is available in the [keynote slides](https://cs.nyu.edu/~panozzo/libigl/libigl-course-slides.key.zip).

## Part 1: libigl overview

* 1.1 [How to clone the repository](https://libigl.github.io/tutorial/#chapter-1)

* 1.2 [Structure of libigl](https://github.com/libigl/libigl)

* 1.3 [Tutorial Compilation](https://libigl.github.io/tutorial/#chapter-1)

* 1.4 [Structure of a simple libigl application (Tutorial 102)](https://libigl.github.io/tutorial/#visualizing-surfaces)
* 1.5 [Overview of Discrete Geometric Quantities and Operators (202,203,204,205)](https://libigl.github.io/tutorial/#chapter-2-discrete-geometric-quantities-and-operators)
* 1.6 [Overview of Shape Deformation (401,403,404,405)](https://libigl.github.io/tutorial/#chapter-4-shape-deformation)
* 1.7 [Overview of Mesh Parametrization (501,502,503,505,710)](https://libigl.github.io/tutorial/#chapter-5-parametrization)

## Part 2: How to use libigl as a header-only library

Libigl can be used as a header-only library, you can find an example of a simple application that computes mesh statistics in src/main_solution.cpp.

## Part 3: How to use libigl with libigl-example-project

The recommended way to use libigl is by using the [libigl-example-project](https://github.com/libigl/libigl-example-project) repository. 

## Part 4: Vector Field Design with libigl

A complete example of how to code a simple field interpolation method using libigl. The source is available [here](https://github.com/libigl/libigl-course/tree/master/vector-field-design).

## Part 5: Advanced features

* 4.1 [Serialization](https://libigl.github.io/tutorial/#state-serialization)
* 4.2 [Booleans](https://libigl.github.io/tutorial/#boolean-operations-on-meshes)
* 4.3 [Python](https://libigl.github.io/python-bindings/)

## Part 6: Coding Tips

This is a short list of coding tips that will greatly reduce your pain and suffering before (and after) the SIGGRAPH deadline.

* [Coding Tips](https://libigl.github.io/coding-guidelines/)


Released under the MPL2 License,
Copyright Alec Jacobson and Daniele Panozzo
