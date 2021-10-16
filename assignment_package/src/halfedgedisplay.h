#ifndef HALFEDGEDISPLAY_H
#define HALFEDGEDISPLAY_H

#pragma once
#include "drawable.h"
#include "halfedge.h"

using namespace glm;
using namespace std;

class HalfEdgeDisplay : public Drawable
{
protected:
    HalfEdge *representedHalfEdge;

public:
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;
    // Change which Vertex representedVertex points to
    void updateHalfEdge(HalfEdge*);

    // constructor
    HalfEdgeDisplay(OpenGLContext* context);
    ~HalfEdgeDisplay() override;
    GLenum drawMode() override;
};

#endif // HALFEDGEDISPLAY_H
