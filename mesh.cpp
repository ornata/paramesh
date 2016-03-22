#include "mesh.hpp"

/*
 * Read in a mesh file and initialize a triangleMesh using it.
 * 1st 4B of file: # vertices
 * 2nd 4B of file: # triangles
 * Next # vertices * sizeof(meshVertex) B: vertex data
 * Next # triangles * sizeof(meshTriangle) B: triangle data
 */
void ReadMesh(TriangleMesh &tm, const string &fname)
{
    cout << "Loading mesh file '" << fname << "'...\n";
    FILE* meshFile = fopen(fname.c_str(), "rb");
    if (!meshFile) {
        cerr << "Could not open '" << fname << "'.\n";
        exit(-1);
    }

    // Read in the number of vertices and then the number of triangles
    if (fread(&tm.nv, 4, 1, meshFile) < 1) {
        cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    if (fread(&tm.nt, 4, 1, meshFile) < 1) {
        cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    // Allocate memory for the vertex and triangle arrays
    tm.vertexArray = (MeshVertex*) malloc(tm.nv * sizeof(MeshVertex));
    if (!tm.vertexArray) {
        cerr << "ERROR: Could not allocate memory for vertexArray. (number of vertices = " << tm.nv << ")\n";
        exit(-1);
    }

    tm.triangleArray = (MeshTriangle*) malloc(tm.nt * sizeof(MeshTriangle));
    if (!tm.triangleArray) {
        cerr << "ERROR: Could not allocate memory for triangleArray. (number of triangles = " << tm.nt << ")\n";
        exit(-1);
    }

    // Read in the vertexArray and the triangleArrays
    if (fread(tm.vertexArray, sizeof(uint32_t), tm.nv * 8, meshFile) < (tm.nv * 8)) {
        cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    if (fread(tm.triangleArray, sizeof(uint32_t), tm.nt * 3, meshFile) < (tm.nt * 3)) {
        cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    fclose(meshFile);
}

/* Write a triangle mesh to a file */
void WriteMesh(const TriangleMesh &tm, const string &fname)
{
    FILE* meshFile = fopen(fname.c_str(), "wb");
    if (!meshFile) {
        cerr << "Could not open '" << fname << "' for writing.\n";
        exit(-1);
    }

    // Read in the number of vertices and then the number of triangles
    if (fwrite(&tm.nv, 4, 1, meshFile) < 1) {
        cerr << "ERROR: Could not write to mesh file. \n";
        exit(-1);
    }

    if (fwrite(&tm.nt, 4, 1, meshFile) < 1) {
        cerr << "ERROR: Could not write to mesh file. \n";
        exit(-1);
    }

    // Read in the vertexArray and the triangleArrays
    if (fwrite(tm.vertexArray, sizeof(uint32_t), tm.nv * 8, meshFile) < (tm.nv * 8)) {
        cerr << "ERROR: Could not write to mesh file. \n";
        exit(-1);
    }

    if (fwrite(tm.triangleArray, sizeof(uint32_t), tm.nt * 3, meshFile) < (tm.nt * 3)) {
        cerr << "ERROR: Could not write to mesh file. \n";
        exit(-1);
    }

    fclose(meshFile);
}

/*
* Generate points on a surface by stepping along it in discrete horizontal
* and vertical steps using two angles, theta and phi. This results in points
* on the surface coming out as squares, which can then be used to triangulate
* the surface. The angle theta steps along the slices (vertical cuts), and the
* angle phi steps along the rings (horizontal cuts).
* ---------------------------- Parameters ----------------------------
* rings:  The number of horizontal cuts to make in the surface
* slices: The number of vertical cuts to make in the surface
* pt_fn:  The parametric function for the surface
* pstep:  The step size for the angle phi
* tstep:  The step size for the angle theta
*/
void GeneratePoints(vector<MeshVertex> &vlist,
                    const int &rings, const int &slices,
                    function<vec3(float, float)> pt_fn,
                    const float pstep, const float tstep)
{
    float theta = 0.0f;
    float umap = 1.0f/((float)rings * pstep);
    float vmap = 1.0f/((float)slices * tstep);

    for (int i = 0; i <= slices; i++) {
        float phi = 0.0f;
        for (int j = 0; j <= rings; j++) {
            MeshVertex v;
            vec3 pt = pt_fn(theta, phi); // get pt on surface
            v.position[0] = pt.x;
            v.position[1] = pt.y;
            v.position[2] = pt.z;

            // map texture coords to surface
            v.tex_coord[0] = phi * umap;
            v.tex_coord[1] = theta * vmap;
            vlist.push_back(v);
            phi += pstep;
        }
        theta += tstep;
    }
}

/*
* Compute the triangles for the surface
*
* Memory for tlist is laid out like
* <0...nrings><0...nrings><0...nrings>...<0...nrings>, where each <...>
* corresponds to a single slice.
* From this, we can figure out what the quad is that we formed for the surface
* and split it into two triangles.
*/
void GenerateFaces(vector<MeshTriangle> &tlist, const int &nrings, const int &nslices)
{
    for (int pt = 0; pt < nslices * (nrings+1); pt += (nrings+1)) {
        for (int curr_ring = 0; curr_ring <= nrings; curr_ring++) {

            /* form a quad:
            * pt+next_ring--------------pt+next_ring+nrings
            *     |                            |
            *     |                            |
            *     |                            |
            * pt+curr_ring--------------pt+curr_ring+nrings
            */

            MeshTriangle t1, t2;
            int next_ring = (curr_ring+1) % (nrings+1);

            /*
            * triangle formed by
            * i0 = pt+curr_ring, i1 = pt+next_ring, i2 = pt+curr_ring+nrings
            * i1
            *  | \
            *  |  \
            * i0---i2
            */
            t1.i0 = pt + curr_ring;
            t1.i1 = pt + next_ring;
            t1.i2 = pt + curr_ring + (nrings+1);
            tlist.push_back(t1);

            /*
            * in this triangle, i0 = prev triangle's i1
            * triangle formed by
            * i0 = pt+next_ring, i1 = pt+next_ring+nrings, i2 = pt+curr_ring+nrings
            * i0 ---i1
            *   \   |
            *    \  |
            *      i2
            */
            t2.i0 = t1.i1;
            t2.i1 = pt + next_ring + (nrings+1);
            t2.i2 = t1.i2;
            tlist.push_back(t2);
        }
    }
}

/*
* Compute the vertex normals for a surface as the weighted average of the
* vertex's incident face normals. Incident normals contribute to the average
* if the cosine of the angle between them is > 0 (they aren't perpendicular).
* The contribution of each triangle other than the first one the vertex appears
* in is weighted by the area of that triangle.
*/
void GenerateVertexNormals(vector<MeshVertex> &vlist, const vector<MeshTriangle> &tlist)
{
    // helper lambda to find the area of a triangle
    auto area = [&vlist](MeshTriangle t)
    {
        vec3 p0 = vec3(vlist[t.i0].vx, vlist[t.i0].vy, vlist[t.i0].vz);
        vec3 p1 = vec3(vlist[t.i1].vx, vlist[t.i1].vy, vlist[t.i1].vz);
        vec3 p2 = vec3(vlist[t.i2].vx, vlist[t.i2].vy, vlist[t.i2].vz);

        // every triangle is ~roughly~ a right triangle
        return 0.5f *  length(p2-p0) *  length(p1-p0);
    };

    // helper lambda to get a normal for a triangle
    auto get_normal = [&vlist](int i0, int i1, int i2)
    {
        vec3 p0 = vec3(vlist[i0].vx, vlist[i0].vy, vlist[i0].vz);
        vec3 p1 = vec3(vlist[i1].vx, vlist[i1].vy, vlist[i1].vz);
        vec3 p2 = vec3(vlist[i2].vx, vlist[i2].vy, vlist[i2].vz);

        return normalize(cross(p1-p0, p2-p0));
    };

    // loop over vertices and find the vertex normal for the vertex
    #pragma omp parallel for
    for (int v = 0; v < vlist.size(); v++) {

        // find the first triangle the vertex appears in
        for (int i = 0; i < tlist.size(); i++) {
            if ((tlist[i].i0 != v) &&
                (tlist[i].i1 != v) &&
                (tlist[i].i2 != v)) continue;

            vec3 n = get_normal(tlist[i].i0, tlist[i].i1, tlist[i].i2);

            // average n with every other triangle that the vertex appears in
            for (int j = 0; j < tlist.size(); j++) {
                if (i == j) continue;
                if ((tlist[j].i0 != v) && (tlist[j].i1 != v) && (tlist[j].i2 != v)) continue;

                vec3 q = get_normal(tlist[j].i0, tlist[j].i1, tlist[j].i2);
                if (dot(n, q) > 0.1f) {
                    n += area(tlist[j]) * q;
                }
            }

            n = normalize(n);
            vlist[v].nx = n.x;
            vlist[v].ny = n.y;
            vlist[v].nz = n.z;

            break; // move to the next vertex
        }
    }
}

/*
* Computing the verted normals of a sphere can be done easily by just
* taking point-origin for each point on the sphere.
*/
void GenerateSphereVertexNormals(vector<MeshVertex> &vlist)
{
    for (int i = 0; i < vlist.size(); i++)
    {
        // get the normal for the pt
        vec3 n = normalize(vec3(
            vlist[i].vx,
            vlist[i].vy,
            vlist[i].vz
        ));

        // set the normal
        vlist[i].nx = n.x;
        vlist[i].ny = n.y;
        vlist[i].nz = n.z;
    }
}
