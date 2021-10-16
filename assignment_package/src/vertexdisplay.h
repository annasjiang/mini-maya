#ifndef VERTEXDISPLAY_H
#define VERTEXDISPLAY_H

#pragma once
#include "drawable.h"
#include "vertex.h"

using namespace glm;
using namespace std;

class VertexDisplay : public Drawable {
public:
    Vertex *representedVertex;

    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;
    // Change which Vertex representedVertex points to
    void updateVertex(Vertex*);

    // constructor
    VertexDisplay(OpenGLContext* context);
    ~VertexDisplay() override;
    GLenum drawMode() override;


};

#endif // VERTEXDISPLAY_H
