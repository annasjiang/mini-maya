#ifndef FACE_H
#define FACE_H

#pragma once
#include <glm/glm.hpp>
#include "halfedge.h"
#include "QListWidget"

class Vertex;
class HalfEdge;

using namespace glm;

static int faceID; // keep track of ids

class Face : public QListWidgetItem {
public:
    // member vars
    int id; // unique identification
    HalfEdge* halfEdge; // ptr to a half edge tht points to this face
    vec3 color; // color of face
    QString name; // for GUI

    // constructors
    Face();
    Face(vec3 c, HalfEdge* hE);
    Face(const Face &f);

    // function to reset ids when importing
    static void resetFaceID();
};

#endif // FACE_H
