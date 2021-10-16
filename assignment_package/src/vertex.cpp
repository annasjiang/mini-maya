#include "vertex.h"

Vertex::Vertex()
    : QListWidgetItem(), id(vertID++), position(vec3()), halfEdge(nullptr),
      name("vert "), jointInfluences()
{
    name.append(QString::number(id));
    this->setText(name);
}

Vertex::Vertex(vec3 p, HalfEdge* hE)
    : QListWidgetItem(), id(vertID++), position(p), halfEdge(hE), name("vert "),
      jointInfluences()
{
    name.append(QString::number(id));
    this->setText(name);
}

Vertex::Vertex(const Vertex &v)
    : QListWidgetItem(), id(v.id), position(v.position), halfEdge(v.halfEdge),
       name(v.name), jointInfluences(v.jointInfluences)
{}

void Vertex::resetVertID() {
    vertID = 0;
}
