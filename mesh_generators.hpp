#ifndef MESH_GENERATORS_HPP
#define MESH_GENERATORS_HPP

#include "mesh.hpp"

TriangleMesh GenerateParametricSphereMesh(const int &rings, const int &slices);
TriangleMesh GenerateParametricKleinMesh(const int &rings, const int &slices);
TriangleMesh GenerateParametricTorusMesh(const int &rings, const int &slices);
TriangleMesh GenerateBagelKleinMesh(const int &rings, const int &slices);

#endif // MESH_GENERATORS_HPP
