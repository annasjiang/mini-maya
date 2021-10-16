#include "skeleton.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

Skeleton::Skeleton(OpenGLContext* context)
    : Drawable(context), representedJoint(-1)
{}

Skeleton::~Skeleton()
{}

void Skeleton::create() {
    vector<GLuint> idx;
    vector<vec4> pos;
    vector<vec4> col;
    vec4 offset;

    int last_idx = -1;
    for (unsigned int i = 0; i < joints.size(); i++) {
        Joint* joint = joints[i].get();
        int start_idx = last_idx + 1;

        // y circle
        offset = vec4(0, 0.5, 0, 1);
        for (int j = 0; j < 12; j++) {
            pos.push_back(joint->getOverallTransformation() * offset);
            if (i == representedJoint) {
                col.push_back(vec4(1, 1, 1, 1));
            } else {
                col.push_back(vec4(1, 0, 0, 1));
            }
            offset = rotate(offset, radians(30.f), vec3(1, 0, 0));

            if (j == 11) {
                idx.push_back(GLuint(start_idx + j));
                idx.push_back(GLuint(start_idx));
            } else {
                idx.push_back(GLuint(start_idx + j));
                idx.push_back(GLuint(start_idx + j + 1));
            }
        }

        // z circle
        offset = vec4(0, 0, 0.5, 1);
        for (int j = 12; j < 24; j++) {
            pos.push_back(joint->getOverallTransformation() * offset);
            if (i == representedJoint) {
                col.push_back(vec4(1, 1, 1, 1));
            } else {
                col.push_back(vec4(0, 1, 0, 1));
            }
            offset = rotate(offset, radians(30.f), vec3(0, 1, 0));

            if (j == 23) {
                idx.push_back(GLuint(start_idx + j));
                idx.push_back(GLuint(start_idx + 12));
            } else {
                idx.push_back(GLuint(start_idx + j));
                idx.push_back(GLuint(start_idx + j + 1));
            }
        }

        // x circle
        offset = vec4(0.5, 0, 0, 1);
        for (int j = 24; j < 36; j++) {
            pos.push_back(joint->getOverallTransformation() * offset);
            if (i == representedJoint) {
                col.push_back(vec4(1, 1, 1, 1));
            } else {
                col.push_back(vec4(0, 0, 1, 1));
            }
            offset = rotate(offset, radians(30.f), vec3(0, 0, 1));

            if (j == 35) {
                last_idx = start_idx + j;
                idx.push_back(GLuint(start_idx + j));
                idx.push_back(GLuint(start_idx + 24));
            } else {
                idx.push_back(GLuint(start_idx + j));
                idx.push_back(GLuint(start_idx + j + 1));
            }
        }

        // connecting lines
        if (joint->parent != nullptr) {
            pos.push_back(joint->getOverallTransformation() * vec4(0, 0, 0, 1));
            pos.push_back(joint->parent->getOverallTransformation() * vec4(0, 0, 0, 1));
            col.push_back(vec4(1, 1, 0, 1)); // gradient color
            col.push_back(vec4(1, 0, 1, 1));
            idx.push_back(start_idx + 36);
            idx.push_back(start_idx + 37);
            last_idx = start_idx + 37;
        }

        start_idx = last_idx;
    }

    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(vec4), col.data(), GL_STATIC_DRAW);
}

GLenum Skeleton::drawMode() {
    return GL_LINES;
}

void Skeleton::updateJoint(Joint* joint) {
    representedJoint = joint->id;
}
