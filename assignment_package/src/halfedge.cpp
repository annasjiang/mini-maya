#include "halfedge.h"

HalfEdge::HalfEdge()
    : QListWidgetItem(), id(halfEdgeID++), next(nullptr), sym(nullptr),
      face(nullptr), vertex(nullptr),  name("hEdge ")
{
    name.append(QString::number(id));
    this->setText(name);
}

HalfEdge::HalfEdge(HalfEdge* n, HalfEdge* s, Face* f, Vertex* v)
    : QListWidgetItem(), id(halfEdgeID++), next(n), sym(s), face(f),
      vertex(v), name("hEdge ")
{
    name.append(QString::number(id));
    this->setText(name);
}

HalfEdge::HalfEdge(const HalfEdge &he)
    : QListWidgetItem(), id(he.id), next(he.next), sym(he.sym),
      face(he.face), vertex(he.vertex), name(he.name)
{}

void HalfEdge::setFace(Face *f) {
    this->face = f;
    f->halfEdge = this;
}

void HalfEdge::setVertex(Vertex *v) {
    this->vertex = v;
    v->halfEdge = this;
}

void HalfEdge::resetHalfEdgeID() {
    halfEdgeID = 0;
}
