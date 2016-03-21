# paramesh
Generate a triangle mesh from a parametric equation. This is done by taking the surface and "cutting it up" into "rings" and "slices". Two angles, theta and phi step along the rings and the slices to form quads on the mesh, which are then broken up into two triangles.

#### Requirements
Some sort of vector math library. I used glm. You can easily sub your own in by editing mesh.hpp. (I'll probably make this more standalone at some point when I have a bit more time.)

#### Usage
mesh_generators.cpp contains some sample functions for generating meshes. You can easily create your own function (or a general function) by using those functions as a model. To make a function...

1. Create a list of vertices and triangles for the mesh
2. Create a function vec3 f(float u, v) with u, v angles which defines your surface. u is the angle between horizontal cuts in the surface, and v is the angle between vertical cuts in the surface.
3. Call GeneratePoints
4. Call GenerateFaces
5. Call GenerateVertexNormals
6. Copy over the number of vertices, number of triangles, and the lists to the mesh

After, you can use the resulting TriangleMesh type directly, or you can output to a file using WriteMesh.




