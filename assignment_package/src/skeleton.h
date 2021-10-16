#ifndef Skeleton_H
#define Skeleton_H

#pragma once
#include "drawable.h"
#include "joint.h"
#include "smartpointerhelp.h"

using namespace glm;
using namespace std;

class Skeleton : public Drawable { // sim to primative display classes
public:
    vector<uPtr<Joint>> joints;
    int representedJoint;

    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;
    // Change which Vertex representedVertex points to
    void updateJoint(Joint*);

    // constructor
    Skeleton(OpenGLContext* context);
    ~Skeleton() override;
    GLenum drawMode() override;
};

#endif // Skeleton_H
