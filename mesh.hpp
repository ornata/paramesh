#ifndef MESH_HPP
#define MESH_HPP

#include <math.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;
const float pi = 3.1415926f;

/*
 * A vertex read from a .mesh file.
 */
struct MeshVertex {

    // Position coordinates
    union {
        float position[3];
        struct {
            float vx, vy, vz;
        };
    };

    // Texture coordinates
    union {
        float tex_coord[2];
        struct {
            float tx, ty;
        };
    };

    // Normal coordinates
    union {
        float normal[3]; // normal
        struct {
            float nx, ny, nz;
        };
    };
};

/*
 * Triangle in a mesh from a .mesh file. Note that triangles are
 * stored in CW order. The meshTriangle struct stores the index of
 * a vertex in the vertexArray of its triangleMesh.
 */
struct MeshTriangle {
    uint32_t i0; // ID of first vertex in vertexArray
    uint32_t i1; // ID of second vertex in vertexArray
    uint32_t i2; // ID of third vertex in vertexArray
};

struct TriangleMesh
{
    uint32_t nv;                    // number of vertices
    uint32_t nt;                    // number of triangles
    MeshVertex* vertexArray;        // contains every vertex in the mesh
    MeshTriangle* triangleArray;    // contains every triangle in the mesh

    TriangleMesh()
    : nv(0), nt(0), vertexArray(nullptr), triangleArray(nullptr)
    {}

    ~TriangleMesh()
    {
        if (vertexArray) { free(vertexArray); }
        if (triangleArray) { free(triangleArray); }
    }
};

void ReadMesh(TriangleMesh &tm, const string &fname);
void WriteMesh(const TriangleMesh &tm, const string &fname);
void GeneratePoints(vector<MeshVertex> &vlist,
                    const int &rings, const int &slices,
                    function<vec3(float, float)> pt_fn,
                    const float pstep, const float tstep);
void GenerateFaces(vector<MeshTriangle> &tlist, const int &nrings, const int &nslices);
void GenerateVertexNormals(vector<MeshVertex> &vlist, const vector<MeshTriangle> &tlist);
void GenerateSphereVertexNormals(vector<MeshVertex> &vlist);

#endif // MESH_HPP
