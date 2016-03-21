# paramesh
Generate a triangle mesh from a parametric equation. This is done by taking the surface and "cutting it up" into "rings" and "slices". Two angles, theta and phi step along the rings and the slices to form quads on the mesh, which are then broken up into two triangles.

For example, for a sphere we'd break it up like this:

![](http://i.imgur.com/4saK9O4.jpg)

More rings and slices will mean more triangles in the mesh. 32 rings and 32 slices seems to work pretty well for building a generally smooth mesh.

#### Requirements
* Some sort of vector math library. I used glm. You can easily sub your own in by editing mesh.hpp. (I'll probably make this more standalone at some point when I have a bit more time.)
* A C++ compiler that supports at least C++11.


#### Usage
mesh_generators.cpp contains some sample functions for generating meshes. You can easily create your own function (or a general function) by using those functions as a model. To make a function...

1. Create a list of vertices and triangles for the mesh
2. Create a function <code>vec3 f(float u, v)</code> with <code>u</code>, <code>v</code> angles in radians which defines your surface. <code>u</code> is the angle between horizontal cuts in the surface (theta), and <code>v</code> is the angle between vertical cuts in the surface.
3. Call <code>GeneratePoints</code> (generates points and texture coordinates)
4. Call <code>GenerateFaces</code> (generates triangles for the mesh)
5. Call <code>GenerateVertexNormals</code> (generates vertex normals for the mesh)
6. Copy over the number of vertices, number of triangles, and the lists to the mesh

After, you can use the resulting <code>TriangleMesh</code> type directly, or you can output to a file by calling <code>WriteMesh</code>.




