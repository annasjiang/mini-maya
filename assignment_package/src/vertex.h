#ifndef VERTEX_H
#define VERTEX_H

#pragma once
#include "glm/glm.hpp"
#include "halfedge.h"
#include "QListWidget"
#include "joint.h"

class Face;
class HalfEdge;

using namespace glm;
using namespace std;

static int vertID; // keep track of ids

class Vertex : public QListWidgetItem {
public:
    // member vars
    int id; // unique identification
    vec3 position; // position of vertex
    HalfEdge* halfEdge; // ptr to a half edge tht points to this vertex
    QString name; // for GUI

    vector<pair<int, float>> jointInfluences;

    // constructors
    Vertex();
    Vertex(vec3 p, HalfEdge* hE);
    Vertex(const Vertex &v);

    // function to reset ids when importing
    static void resetVertID();
};

#endif // VERTEX_H
