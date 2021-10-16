#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H

#pragma once
#include "drawable.h"
#include "face.h"

using namespace glm;
using namespace std;

class FaceDisplay : public Drawable {
protected:
    Face *representedFace;

public:
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;
    // Change which Vertex representedVertex points to
    void updateFace(Face*);

    // constructor
    FaceDisplay(OpenGLContext* context);
    ~FaceDisplay() override;
    GLenum drawMode() override;
};

#endif // FACEDISPLAY_H
