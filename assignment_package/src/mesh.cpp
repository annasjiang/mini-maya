#include "mesh.h"

Mesh::Mesh(OpenGLContext* context)
    : Drawable(context), vertexList{}, halfEdgeList{}, faceList{}
{}

Mesh::~Mesh()
{}

void Mesh::create() {
    vector<vec4> pos;
    vector<vec4> nor;
    vector<vec4> color;
    vector<GLuint> indices;

    vector<ivec2> jointID;
    vector<vec2> jointWeight;

    int i = 0;
    for (uPtr<Face> &f : faceList) {
        HalfEdge *startEdge = f->halfEdge;
        HalfEdge *currEdge = startEdge;
        int numSides = 0;
        do {
            pos.push_back(vec4(currEdge->vertex->position, 1)); // position
            color.push_back(vec4(f->color, 1)); // color

            HalfEdge* he = currEdge; // normal
            vec3 v1 = he->next->vertex->position - he->vertex->position;
            vec3 v2 = he->next->next->vertex->position - he->next->vertex->position;
            while (length(cross(v1, v2)) < FLT_EPSILON) { // split edge case
                he = he->next;
                v1 = he->next->vertex->position - he->vertex->position;
                v2 = he->next->next->vertex->position - he->next->vertex->position;
            }
            nor.push_back(vec4(normalize(cross(v1,v2)), 0));

            // joint influence
            Vertex* vertex = currEdge->vertex;
            if (vertex->jointInfluences.size() != 0) {
                jointID.push_back(ivec2(vertex->jointInfluences[0].first, vertex->jointInfluences[1].first));
                jointWeight.push_back(vec2(vertex->jointInfluences[0].second, vertex->jointInfluences[1].second));
            }

            currEdge = currEdge->next; // trav
            numSides++;
        } while (currEdge != startEdge);

        for (int j = 0; j < numSides - 2; j++) {
            indices.push_back(i);
            indices.push_back(i + j + 1);
            indices.push_back(i + j + 2);
        }
        i += numSides;
    }

    count = indices.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(vec4), pos.data(), GL_STATIC_DRAW);

    generateNor();
    bindNor();
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(vec4), color.data(), GL_STATIC_DRAW);

    generateJointID();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufJointID);
    mp_context->glBufferData(GL_ARRAY_BUFFER, jointID.size() * sizeof(ivec2), jointID.data(), GL_STATIC_DRAW);

    generateJointWeight();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufJointWeight);
    mp_context->glBufferData(GL_ARRAY_BUFFER, jointWeight.size() * sizeof(vec2), jointWeight.data(), GL_STATIC_DRAW);
}

GLenum Mesh::drawMode() {
    return GL_TRIANGLES;
}

// helper function to set 2 half edges as sym edges
void Mesh::setSym(int x, int y) {
    halfEdgeList.at(x)->sym = halfEdgeList.at(y).get();
    halfEdgeList.at(y)->sym = halfEdgeList.at(x).get();
}

// helper function to set the vertex half edge points to
void Mesh::setHalfEdgeVert(int x, int y, int z) {
    halfEdgeList.at(x)->vertex = vertexList.at(x).get();
    halfEdgeList.at(y)->vertex = vertexList.at(x).get();
    halfEdgeList.at(z)->vertex = vertexList.at(x).get();
}

void Mesh::createCube() {
    vertexList.clear();
    halfEdgeList.clear();
    faceList.clear();

    for (int i = 0; i < 8; i++) {
        vertexList.push_back(mkU<Vertex>());
    }
    for (int i = 0; i < 24; i++) {
        halfEdgeList.push_back(mkU<HalfEdge>());
    }
    for (int i = 0; i < 6; i++) {
        faceList.push_back(mkU<Face>());
    }

    // VERTEX
    for (int i = 0; i < 8; i++) { // set half-edge
        vertexList.at(i)->halfEdge = halfEdgeList.at(i).get();
    }

    // position
    vertexList.at(0)->position = vec3(-0.5f, 0.5f, 0.5f);
    vertexList.at(1)->position = vec3(-0.5f, -0.5f, 0.5f);
    vertexList.at(2)->position = vec3(0.5f, -0.5f, 0.5f);
    vertexList.at(3)->position = vec3(0.5f, 0.5f, 0.5f);
    vertexList.at(4)->position = vec3(0.5f, 0.5f, -0.5f);
    vertexList.at(5)->position = vec3(0.5f, -0.5f, -0.5f);
    vertexList.at(6)->position = vec3(-0.5f, -0.5f, -0.5f);
    vertexList.at(7)->position = vec3(-0.5f, 0.5f, -0.5f);

    // HALF-EDGE
    for (int i = 0; i < 24 ; i++) { // set next edge
        if (i % 4 != 3) {
            halfEdgeList.at(i)->next = halfEdgeList.at(i + 1).get();
        } else {
            halfEdgeList.at(i)->next = halfEdgeList.at(i - 3).get();
        }
    }

    // sym edges
    setSym(0, 18);
    setSym(1, 15);
    setSym(2, 22);
    setSym(3, 9);
    setSym(4, 16);
    setSym(5, 11);
    setSym(6, 20);
    setSym(7, 13);
    setSym(8, 19);
    setSym(10, 21);
    setSym(12, 17);
    setSym(14, 23);

    for (int i = 0; i < 24; i++) { // set faces
        halfEdgeList.at(i)->face = faceList.at(i / 4).get();
    }

    // set vertices
    setHalfEdgeVert(0, 15, 17);
    setHalfEdgeVert(1, 14, 22);
    setHalfEdgeVert(2, 9, 21);
    setHalfEdgeVert(3, 8, 18);
    setHalfEdgeVert(4, 11, 19);
    setHalfEdgeVert(5, 10, 20);
    setHalfEdgeVert(6, 13, 23);
    setHalfEdgeVert(7, 12, 16);

    // FACE
    for (int i = 0; i < 6; i++) { // set half edge
        faceList.at(i)->halfEdge = halfEdgeList.at(4 * i).get();
    }

    // color
    faceList.at(0)->color = vec3(0.f, 0.f, 1.f);
    faceList.at(1)->color = vec3(0.f, 0.f, 1.f);
    faceList.at(2)->color = vec3(1.f, 0.f, 0.f);
    faceList.at(3)->color = vec3(1.f, 0.f, 0.f);
    faceList.at(4)->color = vec3(0.f, 1.f, 0.f);
    faceList.at(5)->color = vec3(0.f, 1.f, 0.f);
}

void Mesh::resetCount() {
    Vertex::resetVertID();
    HalfEdge::resetHalfEdgeID();
    Face::resetFaceID();
}
