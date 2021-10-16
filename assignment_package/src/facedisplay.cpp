#include "facedisplay.h"

FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr)
{}

FaceDisplay::~FaceDisplay()
{}

void FaceDisplay::create() {
    vector<vec4> pos;
    vector<vec4> col;
    vector<GLuint> indices;

    HalfEdge *start = representedFace->halfEdge;
    HalfEdge *end = representedFace->halfEdge;

    int i = 0;
    int numSides = 0;
    do {
        pos.push_back(vec4(start->vertex->position, 1));
        col.push_back(vec4(vec3(1, 1, 1) - representedFace->color, 1)); // flip color

        start = start->next;
        numSides++;
    } while (start != end);

    for (int i = 0; i < numSides; i++) {
        indices.push_back(i);
        indices.push_back((i + 1) % numSides);
    }

    count = indices.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);
}

GLenum FaceDisplay::drawMode() {
    return GL_LINES;
}

void FaceDisplay::updateFace(Face* f) {
    representedFace = f;
}
