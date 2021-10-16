#ifndef HALFEDGE_H
#define HALFEDGE_H

#pragma once
#include <glm/glm.hpp>
#include "smartpointerhelp.h"
#include "face.h"
#include "vertex.h"
#include "QListWidget"

class Face;
class Vertex;

using namespace glm;

static int halfEdgeID; // keep track of ids

class HalfEdge : public QListWidgetItem {
public:
    // member vars
    int id; // unique id
    HalfEdge* next; // ptr to net half edge in loop
    HalfEdge* sym; // ptr to symetric half edge
    Face* face; // ptr to face
    Vertex* vertex; // ptr to vertex between this half edge and next
    QString name; // for GUI

    // constructors
    HalfEdge();
    HalfEdge(HalfEdge* n, HalfEdge* s, Face* f, Vertex* v);
    HalfEdge(const HalfEdge &he);

    // functions
    void setFace(Face *f);
    void setVertex(Vertex *v);
    static void resetHalfEdgeID();
};

#endif // HALFEDGE_H
