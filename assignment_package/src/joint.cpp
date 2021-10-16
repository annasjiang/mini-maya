#include "joint.h"

Joint::Joint()
    : QTreeWidgetItem(), id(jointID++), name(), parent(nullptr), children(),
      position(vec4(0.f, 0.f, 0.f, 1.f)), rotation(quat(1.f, 0.f, 0.f, 0.f)), bind_matrix(mat4(1.f))
{
    this->QTreeWidgetItem::setText(0, this->name);
}

Joint::Joint(QString name)
    : QTreeWidgetItem(), id(jointID++), name(name), parent(nullptr), children(),
      position(vec4(0, 0, 0, 1)), rotation(quat(1, 0, 0, 0)), bind_matrix(mat4(1.f))
{
    this->QTreeWidgetItem::setText(0, name);
}

Joint::Joint(QString name, Joint *parent, vec3 position, quat rotation)
    : QTreeWidgetItem(), id(jointID++), name(name), parent(parent), children(vector<Joint*>()),
      position(position), rotation(rotation), bind_matrix(mat4(1.0f))
{
    this->QTreeWidgetItem::setText(0, name);
}

Joint::Joint(const Joint &j)
    : QTreeWidgetItem(), id(j.id), name(j.name), parent(j.parent), children(j.children),
      position(j.position), rotation(j.rotation), bind_matrix(j.bind_matrix)
{
    this->QTreeWidgetItem::setText(0, name);
}


Joint::~Joint() {}

mat4 Joint::getLocalTransformation() {
    mat4 T = mat4(vec4(1.f, 0.f, 0.f, 0.f),
                  vec4(0.f, 1.f, 0.f, 0.f),
                  vec4(0.f, 0.f, 1.f, 0.f),
                  vec4(this->position, 1.f));
    mat4 R = toMat4(this->rotation);

    return T * R;
}

mat4 Joint::getOverallTransformation() {
    if (parent == nullptr) {
        return this->getLocalTransformation();
    } else {
        return parent->getOverallTransformation() * this->getLocalTransformation();
    }
}

void Joint::resetJointID() {
    jointID = 0;
}
