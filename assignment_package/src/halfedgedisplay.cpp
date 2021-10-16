#include "halfedgedisplay.h"

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext *context)
    : Drawable(context), representedHalfEdge(nullptr)
{}

HalfEdgeDisplay::~HalfEdgeDisplay()
{}

void HalfEdgeDisplay::create() {
    vector<vec4> pos;
    vector<vec4> col;
    vector<GLuint> indices;

    HalfEdge *start = representedHalfEdge->sym;
    HalfEdge *end = representedHalfEdge;

    pos.push_back(vec4(start->vertex->position, 1));
    pos.push_back(vec4(end->vertex->position, 1));

    col.push_back(vec4(1, 0, 0, 1)); // gradient color
    col.push_back(vec4(1, 1, 0, 1));

    indices.push_back(0);
    indices.push_back(1);

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

GLenum HalfEdgeDisplay::drawMode() {
    return GL_LINES;
}

void HalfEdgeDisplay::updateHalfEdge(HalfEdge* hE) {
    representedHalfEdge = hE;
}
