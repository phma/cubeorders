# Overview
Cubeorders is a program for computing the number of possible orders of the corners
of a hypercube by distance from a point which is not equidistant from any two
corners. It computes the correct number up to five dimensions. In six dimensions,
it takes a very long time (at least a day on my box, compared to six seconds in
five dimensions).

# Compiling
First install Quadlods, which Cubeorders uses to generate n-tuples, where n is the
number of dimensions. You will need GMP and Boost for both Quadlods and Cubeorders.

To compile, if you're not developing the program:

1. Create a subdirectory build/ inside the directory where you untarred the source code.
2. `cd build`
3. `cmake ..`
4. `make`

If you are developing the program:

1. Create a directory build/cubeorders outside the directory where you cloned the source code.
2. `cd build/cubeorders`
3. `cmake <directory where the source code is>`
4. `make`
