/* Some example mesh functions */
#include "mesh_generators.hpp"

TriangleMesh GenerateParametricSphereMesh(const int &rings, const int &slices)
{
    vector<MeshTriangle> tlist; // contains triangles for mesh
    vector<MeshVertex> vlist;   // contains vertices for mesh

    // parametric eq for a point on a sphere given u = theta, v = phi
    auto sphere_pt = [](float u, float v) {
        return vec3(
        sin(u)*cos(v),
        cos(u),
        -sin(u)*sin(v));
    };

    GeneratePoints(vlist, rings, slices, sphere_pt, 2.0f*pi/(float)rings, pi/(float)slices);
    GenerateFaces(tlist, rings, slices);
    GenerateSphereVertexNormals(vlist);

    TriangleMesh sphere;
    sphere.nv = vlist.size();
    sphere.nt = tlist.size();
    sphere.vertexArray = (MeshVertex*) malloc(vlist.size() * sizeof(MeshVertex));
    sphere.triangleArray = (MeshTriangle*) malloc(tlist.size() * sizeof(MeshTriangle));

    copy(vlist.begin(), vlist.begin() + vlist.size(), sphere.vertexArray);
    copy(tlist.begin(), tlist.begin() + tlist.size(), sphere.triangleArray);

    return sphere;
}


TriangleMesh GenerateParametricKleinMesh(const int &rings, const int &slices)
{
    vector<MeshTriangle> tlist; // contains triangles for mesh
    vector<MeshVertex> vlist;   // contains vertices for mesh

    // Equation for a point on a Klein bottle given u = theta, v = phi
    auto klein_pt = [](float u, float v) {
        float x, y, z;

        if (u < pi) {
            x = 3.0f*cos(u)*(1.0f+sin(u))+(2.0f*(1.0f-cos(u)/2.0f))*cos(u)*cos(v);
            z = -8.0f*sin(u)-2.0f*(1.0f-cos(u)/2.0f)*sin(u)*cos(v);
        }

        else {
            x = 3.0f*cos(u)*(1.0f+sin(u))+(2.0f*(1.0f-cos(u)/2.0f))*cos(v+pi);
            z = -8.0f*sin(u);
        }

        y = -2.0f*(1.0f-cos(u)/2.0f)*sin(v);
        return vec3(x,y,z);
    };

    GeneratePoints(vlist, rings, slices, klein_pt, 2.0f*pi/(float)rings, 2.0f*pi/(float)slices);
    GenerateFaces(tlist, rings, slices);
    GenerateVertexNormals(vlist, tlist);

    TriangleMesh klein;
    klein.nv = vlist.size();
    klein.nt = tlist.size();
    klein.vertexArray = (MeshVertex*) malloc(vlist.size() * sizeof(MeshVertex));
    klein.triangleArray = (MeshTriangle*) malloc(tlist.size() * sizeof(MeshTriangle));

    copy(vlist.begin(), vlist.begin() + vlist.size(), klein.vertexArray);
    copy(tlist.begin(), tlist.begin() + tlist.size(), klein.triangleArray);

    return klein;
}


TriangleMesh GenerateParametricTorusMesh(const int &rings, const int &slices)
{
    vector<MeshTriangle> tlist; // contains triangles for mesh
    vector<MeshVertex> vlist;   // contains vertices for mesh

    auto torus_pt = [](float u, float v) {
        float x, y, z;
        x = (1 + 0.5*cos(u)) * cos(v);
        y = (1 + 0.5*cos(u)) * sin(v);
        z = 0.5 * sin(u);
        return vec3(x,y,z);
    };

    GeneratePoints(vlist, rings, slices, torus_pt, 2.0f*pi/(float)rings, 2.0f*pi/(float)rings);
    GenerateFaces(tlist, rings, slices);
    GenerateVertexNormals(vlist, tlist);

    TriangleMesh torus;
    torus.nv = vlist.size();
    torus.nt = tlist.size();
    torus.vertexArray = (MeshVertex*) malloc(vlist.size() * sizeof(MeshVertex));
    torus.triangleArray = (MeshTriangle*) malloc(tlist.size() * sizeof(MeshTriangle));

    copy(vlist.begin(), vlist.begin() + vlist.size(), torus.vertexArray);
    copy(tlist.begin(), tlist.begin() + tlist.size(), torus.triangleArray);

    return torus;
}


TriangleMesh GenerateBagelKleinMesh(const int &rings, const int &slices)
{
    vector<MeshTriangle> tlist; // contains triangles for mesh
    vector<MeshVertex> vlist;   // contains vertices for mesh

    auto klein_pt = [](float u, float v) {
        float x, y, z;
        x = (1.0f + cos(v/2.0f)*sin(u) - sin(v/2.0f)*sin(2.0f*u)) * cos(v);
        y = (1 + cos(v/2.0f)*sin(u) - sin(v/2.0f)*sin(2.0f*u)) * sin(v);
        z = sin(v/2.0f)*sin(u) + cos(v/2.0f)*sin(2.0f*u);
        return vec3(x,y,z);
    };

    GeneratePoints(vlist, rings, slices, klein_pt, 2.0f*pi/(float)rings, 2.0f*pi/(float)rings);
    GenerateFaces(tlist, rings, slices);
    GenerateVertexNormals(vlist, tlist);

    TriangleMesh klein;
    klein.nv = vlist.size();
    klein.nt = tlist.size();
    klein.vertexArray = (MeshVertex*) malloc(vlist.size() * sizeof(MeshVertex));
    klein.triangleArray = (MeshTriangle*) malloc(tlist.size() * sizeof(MeshTriangle));

    copy(vlist.begin(), vlist.begin() + vlist.size(), klein.vertexArray);
    copy(tlist.begin(), tlist.begin() + tlist.size(), klein.triangleArray);

    return klein;
}
