#include "face.h"

Face::Face()
    : QListWidgetItem(), id(faceID++), halfEdge(nullptr),
      color(vec3(0, 0, 0)), name("face ")
{
    name.append(QString::number(id));
    this->setText(name);
}

Face::Face(vec3 c, HalfEdge* hE)
    : QListWidgetItem(), id(faceID++), halfEdge(hE), color(c), name("face ")
{
    name.append(QString::number(id));
    this->setText(name);
}

Face::Face(const Face &f)
    : QListWidgetItem(), id(f.id), halfEdge(f.halfEdge), color(f.color),
      name(f.name)
{}

void Face::resetFaceID() {
    faceID = 0;
}
