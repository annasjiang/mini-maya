#ifndef JOINT_H
#define JOINT_H
#include <glm/glm.hpp>
#include <la.h>
#include <QComboBox>
#include <glm/gtx/quaternion.hpp>
#include "QTreeWidget"

using namespace glm;
using namespace std;

static int jointID; // keep track of ids

class Joint : public QTreeWidgetItem {
public:
    int id;
    QString name;
    Joint *parent;
    vector<Joint*> children;
    vec3 position;
    quat rotation;
    mat4 bind_matrix;

    // constructors + destructor
    Joint();
    Joint(QString name);
    Joint(QString name, Joint *parent, vec3 position, quat rotation);
    Joint(const Joint &j);
    ~Joint();

    // functions
    mat4 getLocalTransformation();
    mat4 getOverallTransformation();

    // function to reset ids when importing
    static void resetJointID();
};

#endif // JOINT_H
