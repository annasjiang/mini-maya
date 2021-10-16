#ifndef MESH_H
#define MESH_H

#pragma once
#include "smartpointerhelp.h"
#include "face.h"
#include "vertex.h"
#include "halfedge.h"
#include "drawable.h"
#include "skeleton.h"

using namespace glm;
using namespace std;

class Mesh : public Drawable {
public:
    // member vars
    vector<uPtr<Vertex>> vertexList;
    vector<uPtr<HalfEdge>> halfEdgeList;
    vector<uPtr<Face>> faceList;

    // constructor + destructor
    Mesh(OpenGLContext* context);
    ~Mesh() override;

    // functions
    void create() override;
    GLenum drawMode() override;
    void setSym(int x, int y); // helper
    void setHalfEdgeVert(int x, int y, int z); // helper
    void createCube();
    static void resetCount(); // for importing objs
};

#endif // MESH_H
