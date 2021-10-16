#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog>

#include <set>
#include <queue>
#include <random>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this), m_progLambert(this), m_progFlat(this), m_progSkeleton(this), m_glCamera(),
      m_mesh(this), m_skeleton(this), m_vertex(nullptr), m_vertexDisplay(this), m_face(nullptr),
      m_faceDisplay(this), m_halfEdge(nullptr), m_halfEdgeDisplay(this), m_selected(nullptr),
      highlightMode(false), spinBoxColor(vec3())
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_mesh.destroy();
    m_skeleton.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
//    m_geomSquare.create(); // og thingies
    m_mesh.createCube();
    m_mesh.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    // Create and set up the skeleton shader
    m_progSkeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    emit sig_sendMesh(&m_mesh);
    emit sig_sendSkeleton(&m_skeleton);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progSkeleton.setViewProjMatrix(m_glCamera.getViewProj());

    m_progLambert.setCamPos(glm::vec3(m_glCamera.eye));

    m_progFlat.setModelMatrix(glm::mat4(1.f));
    m_progLambert.setModelMatrix(mat4());
    m_progSkeleton.setModelMatrix(mat4());

    if (m_mesh.vertexList[0]->jointInfluences.size() == 0) {
        m_progFlat.draw(m_mesh);
    } else {
        m_progSkeleton.draw(m_mesh);
    }

    glDisable(GL_DEPTH_TEST); // draw on top
    if (highlightMode) { // handle highlights
        Vertex* vertex = dynamic_cast<Vertex*>(m_selected);
        Face* face = dynamic_cast<Face*>(m_selected);
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(m_selected);

        if (vertex) {
            m_vertexDisplay.create();
            m_progFlat.draw(m_vertexDisplay);
        } else if (face) {
            m_faceDisplay.create();
            m_progFlat.draw(m_faceDisplay);
        } else if (halfEdge) {
            m_halfEdgeDisplay.create();
            m_progFlat.draw(m_halfEdgeDisplay);
        }
    }

    // draw skeleton on top
    m_skeleton.create();
    m_progFlat.draw(m_skeleton);
    glEnable(GL_DEPTH_TEST);
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUpPolar(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUpPolar(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRightPolar(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRightPolar(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLookPolar(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLookPolar(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRightPolar(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRightPolar(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUpPolar(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUpPolar(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) { // begin visual debugging
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(m_selected);
        if (halfEdge != nullptr) {
            slot_select(halfEdge->next);
        }
    } else if (e->key() == Qt::Key_M) {
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(m_selected);
        if (halfEdge != nullptr) {
            slot_select(halfEdge->sym);
        }
    } else if (e->key() == Qt::Key_F) {
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(m_selected);
        if (halfEdge != nullptr) {
            slot_select(halfEdge->face);
        }
    } else if (e->key() == Qt::Key_V) {
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(m_selected);
        if (halfEdge != nullptr) {
            slot_select(halfEdge->vertex);
        }
    } else if ((e->modifiers() & Qt::ShiftModifier) && e->key() == Qt::Key_H) {
        Face* face = dynamic_cast<Face*>(m_selected);
        if (face != nullptr) {
            slot_select(face->halfEdge);
        }
    } else if (e->key() == Qt::Key_H) {
        Vertex* vertex = dynamic_cast<Vertex*>(m_selected);
        if (vertex != nullptr) {
            slot_select(vertex->halfEdge);
        }
    }
    m_glCamera.RecomputeAttributesPolar();
    update();  // Calls paintGL, among other things
}

// SLOTS N SUCH
void MyGL::slot_x(double x) {
    if (m_selected) {
        m_vertex = dynamic_cast<Vertex*>(m_selected);
        if (m_vertex) {
            m_vertex->position[0] = x;

            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_y(double y) {
    if (m_selected) {
        m_vertex = dynamic_cast<Vertex*>(m_selected);
        if (m_vertex) {
            m_vertex->position[1] = y;

            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_z(double z) {
    if (m_selected) {
        m_vertex = dynamic_cast<Vertex*>(m_selected);
        if (m_vertex) {
            m_vertex->position[2] = z;

            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_red(double red) {
    if (m_selected) {
        m_face = dynamic_cast<Face*>(m_selected);
        if (m_face) {
            spinBoxColor[0] = red;
            m_face->color = spinBoxColor;

            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_green(double green) {
    if (m_selected) {
        m_face = dynamic_cast<Face*>(m_selected);
        if (m_face) {
            spinBoxColor[1] = green;
            m_face->color = spinBoxColor;

            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_blue(double blue) {
    if (m_selected) {
        m_face = dynamic_cast<Face*>(m_selected);
        if (m_face) {
            spinBoxColor[2] = blue;
            m_face->color = spinBoxColor;

            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_select(QListWidgetItem *i) {
    if (i != nullptr) {
        m_selected = i;

        Vertex* vertex = dynamic_cast<Vertex*>(i);
        Face* face = dynamic_cast<Face*>(i);
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(i);

        m_vertexDisplay.destroy();
        m_faceDisplay.destroy();
        m_halfEdgeDisplay.destroy();

        if (vertex) {
            m_vertexDisplay.updateVertex(vertex);
        } else if (face) {
            m_faceDisplay.updateFace(face);
        } else if (halfEdge) {
            m_halfEdgeDisplay.updateHalfEdge(halfEdge);
        }

        highlightMode = true;
        update();
    }
}

void MyGL::slot_addVert() { // split edge from notes
    if (m_selected != nullptr) {
        HalfEdge* he1 = dynamic_cast<HalfEdge*>(m_selected);
        if (he1 != nullptr) {
            HalfEdge* he2 = he1->sym;
            uPtr<Vertex> midpt = mkU<Vertex>();
            midpt->position = (he1->vertex->position + he2->vertex->position) / 2.f;
            midpt->halfEdge = he1;

            // split half edge and reassign things
            uPtr<HalfEdge> he1Sym = mkU<HalfEdge>();
            he1Sym->vertex = he1->vertex;
            he1Sym->face = he1->face;
            he1Sym->next = he1->next;
            he1->next = he1Sym.get();
            he1->vertex->halfEdge = he1Sym.get();
            he1->vertex = midpt.get();

            uPtr<HalfEdge> he2Sym = mkU<HalfEdge>();
            he2Sym->vertex = he2->vertex;
            he2Sym->face = he2->face;
            he2Sym->next = he2->next;
            he2->next = he2Sym.get();
            he2->vertex = midpt.get();

            // set sym
            he1->sym = he2Sym.get();
            he2Sym->sym = he1;
            he2->sym = he1Sym.get();
            he1Sym->sym = he2;

            // add new thingz
            m_mesh.vertexList.push_back(std::move(midpt));
            m_mesh.halfEdgeList.push_back(std::move(he1Sym));
            m_mesh.halfEdgeList.push_back(std::move(he2Sym));

            // send data
            m_mesh.destroy();
            m_mesh.create();
            update();
            emit sig_sendMesh(&m_mesh);
        }
    }
}

void MyGL::slot_triangulate() { // triangulate from notes
    if (m_selected != nullptr) {
        Face* face1 = dynamic_cast<Face*>(m_selected);
        if (face1 != nullptr) {
            HalfEdge* start = face1->halfEdge;
            HalfEdge* curr = start;
            int numSides = 0;
            do { // count sides
                curr = curr->next;
                numSides++;
            } while (curr != start);

            for (int i = 0; i < numSides - 3; i++) {
                // 2 new half edges
                HalfEdge* heOG = face1->halfEdge; // original half edge
                uPtr<HalfEdge> he1 = mkU<HalfEdge>();
                uPtr<HalfEdge> he2 = mkU<HalfEdge>();

                he1->vertex = heOG->vertex;
                he1->sym = he2.get();

                he2->vertex = heOG->next->next->vertex;
                he2->sym = he1.get();

                // new face
                uPtr<Face> face2 = mkU<Face>();
                he1->face = face2.get();
                heOG->face = face2.get();
                heOG->next->face = face2.get();
                he2->face = face1;

                face2->halfEdge = he1.get();
                face2->color = face1->color;

                // reassign half edge ptrs
                he2->next = heOG->next->next->next;
                heOG->next->next->next = he1.get();
                he1->next = heOG->next;
                heOG->next = he2.get();

                // add new thingz
                m_mesh.faceList.push_back(std::move(face2));
                m_mesh.halfEdgeList.push_back(std::move(he1));
                m_mesh.halfEdgeList.push_back(std::move(he2));
            }

            // send data
            m_mesh.destroy();
            m_mesh.create();
            update();
            emit sig_sendMesh(&m_mesh);
        }
    }
}

// catmull clark
void MyGL::slot_subdivide() { // from notes
    vector<uPtr<Vertex>> newVerts;
    vector<uPtr<HalfEdge>> newEdges;
    vector<uPtr<Face>> newFaces;

    // half edges to be split
    vector<HalfEdge*> edges2Split;
    initEdges2Split(&edges2Split);

    // 1. COMPUTE CENTROIDS
    map<Face*, Vertex*> centroidMap;
    computeCentroid(&centroidMap, &newVerts);

    // 2. COMPUTE SMOOTHED MIDPOINTS
    map<HalfEdge*, Vertex*> midpointMap;
    computeMidpoints(&centroidMap, &midpointMap, &edges2Split, &newVerts);

    // 3. SMOOTH ORIGINAL VERTICES
    smoothOgVerts(&centroidMap, &midpointMap);

    // 4. QUADRAGULATE
    quadrangulate(&centroidMap, &midpointMap, &edges2Split, &newVerts, &newEdges, &newFaces);

    // send data
    m_mesh.destroy();
    m_mesh.create();
    update();
    emit sig_sendMesh(&m_mesh);
}

// helper tht basically copies the list of half edges we gon b splittin
void MyGL::initEdges2Split(vector<HalfEdge*> *edges2Split) {
    // initialize half-edges to be split
    for (const uPtr<HalfEdge> &he : m_mesh.halfEdgeList) {
        if (std::find(edges2Split->begin(), edges2Split->end(), he.get()->sym) == edges2Split->end()) {
            edges2Split->push_back(he.get());
        }
    }
}

// helper to compute centroids of all faces + map them to corr faces
void MyGL::computeCentroid(map<Face*, Vertex*> *centroidMap,
                           vector<uPtr<Vertex>> *newVerts) {
    for (uPtr<Face> &f : m_mesh.faceList) {
        int numVerts = 0;
        vec3 centroid_pos = vec3();

        HalfEdge *start = f->halfEdge;
        HalfEdge *curr = start;
        uPtr<Vertex> centroid = mkU<Vertex>();

        do { // add all vert positions
            numVerts++;
            centroid_pos += curr->vertex->position;
            curr = curr->next;
        } while (curr != start);

        centroid_pos /= numVerts; // avg verts

        centroid->position = centroid_pos;
        centroidMap->insert(make_pair(f.get(), centroid.get()));
        newVerts->push_back(std::move(centroid));
    }
}

// helper to find midpoints of ea edge
void MyGL::computeMidpoints(map<Face*, Vertex*> *centroidMap,
                            map<HalfEdge*, Vertex*> *midpointMap,
                            vector<HalfEdge*> *edges2Split,
                            vector<uPtr<Vertex>> *newVerts) {

    for (HalfEdge* he : *edges2Split) {
        HalfEdge *prev = he->next;
        while (prev->next != he) {
            prev = prev->next;
        }

        uPtr<Vertex> midPoint = mkU<Vertex>();
        if (he->sym != nullptr) { // reg formula
            midPoint->position = (prev->vertex->position +
                                  he->vertex->position +
                                  centroidMap->find(he->face)->second->position +
                                  centroidMap->find(he->sym->face)->second->position) / 4.f;
        } else { // border edge formula
            midPoint->position = (prev->vertex->position +
                                  he->vertex->position +
                                  centroidMap->find(he->face)->second->position) / 3.f;
        }
        midpointMap->insert(make_pair(he, midPoint.get()));
        newVerts->push_back(std::move(midPoint));
    }
}

// helper to smooth og verts
void MyGL::smoothOgVerts(map<Face*, Vertex*> *centroidMap,
                          map<HalfEdge*, Vertex*> *midpointMap) {
    for (uPtr<Vertex> &v : m_mesh.vertexList) {
        HalfEdge *start = v->halfEdge;
        HalfEdge *curr = start;

        float numAdjMidpts = 0; // n
        vec3 sumAdjMidpts = vec3(); // sum(e)
        vec3 sumCentroids = vec3(); // sum(f)

        do {
            if (midpointMap->find(curr) != midpointMap->end()) {
                sumAdjMidpts += midpointMap->find(curr)->second->position;
            } else {
                sumCentroids += midpointMap->find(curr->sym)->second->position;
            }

            sumCentroids += centroidMap->find(curr->face)->second->position;
            numAdjMidpts++;
            curr = curr->next->sym;
        } while (start != curr);

        v->position = ((numAdjMidpts - 2.f) * v->position) / numAdjMidpts  +
                      sumAdjMidpts / (glm::pow(numAdjMidpts, 2.f)) +
                      sumCentroids / (glm::pow(numAdjMidpts, 2.f));
    }
}

// helper to quadrangluate the face
void MyGL::quadrangulate(map<Face*, Vertex*> *centroidMap,
                        map<HalfEdge*, Vertex*> *midpointMap,
                        vector<HalfEdge*> *edges2Split,
                        vector<uPtr<Vertex>> *newVerts,
                        vector<uPtr<HalfEdge>> *newEdges,
                        vector<uPtr<Face>> *newFaces) {

    for (HalfEdge *he : *edges2Split) { // split all half edges
        splitHalfEdge(he, midpointMap->find(he)->second, newEdges);
    }

    for (uPtr<Face> &f : m_mesh.faceList) {
        vector<HalfEdge*> tempHalfEdge;
        makeTempHalfEdge(f.get(), &tempHalfEdge);
        map<HalfEdge*, HalfEdge*> prevEdgeMap;
        makePrevEdgeMap(&prevEdgeMap, &tempHalfEdge);

        vector<uPtr<HalfEdge>> newH1s; // half edges tht point towards centroid
        vector<uPtr<HalfEdge>> newH2s; // half edges tht point towards midpoint
        vector<uPtr<Face>> faces; // new faces we makin

        // create new halfedges to subdiv
        for (HalfEdge *he : tempHalfEdge) { // trav from lec
            // middle edges tht we add in
            uPtr<HalfEdge> h1 = mkU<HalfEdge>();
            h1->setVertex(centroidMap->find(he->face)->second);
            uPtr<HalfEdge> h2 = mkU<HalfEdge>();
            h2->setVertex(prevEdgeMap.find(he)->second->vertex);

            // complete new ring
            he->next->next = h1.get();
            h1->next = h2.get();
            h2->next = he;

            newH1s.push_back(std::move(h1));
            newH2s.push_back(std::move(h2));
        }

        // match up sym edges
        for (unsigned int i = 0; i < newH1s.size() - 1; i++) {
            newH1s.at(i)->sym = newH2s.at(i + 1).get();
            newH2s.at(i + 1)->sym = newH1s.at(i).get();
        }
        // edge case wrap around
        newH1s.at(newH1s.size() - 1)->sym = newH2s.at(0).get();
        newH2s.at(0)->sym = newH1s.at(newH1s.size() - 1).get();

        // add to new edges
        for (uPtr<HalfEdge> &he : newH1s) {
            newEdges->push_back(std::move(he));
        }
        for (uPtr<HalfEdge> &he : newH2s) {
            newEdges->push_back(std::move(he));
        }

        // create faces and assign edge ring to them
        for (unsigned long i = 0; i < tempHalfEdge.size() - 1; i++) {
            uPtr<Face> face = mkU<Face>();
            float r = (float) rand() / RAND_MAX;
            float g = (float) rand() / RAND_MAX;
            float b = (float) rand() / RAND_MAX;
            face->color = vec3(r, g, b); // random colors eheheh
            assignFace(&tempHalfEdge, i, face.get());
            faces.push_back(std::move(face));
        }
        // edge case wrap around
        assignFace(&tempHalfEdge, tempHalfEdge.size() - 1, f.get());

        // add to new faces
        for (uPtr<Face> &face : faces) {
            newFaces->push_back(std::move(face));
        }
    }

    // add new things to sidebar list
    for (uPtr<Vertex> &v : *newVerts) {
        m_mesh.vertexList.push_back(std::move(v));
    }
    for (uPtr<HalfEdge> &he : *newEdges) {
        m_mesh.halfEdgeList.push_back(std::move(he));
    }
    for (uPtr<Face> &f : *newFaces) {
        m_mesh.faceList.push_back(std::move(f));
    }
}

// helper to split half edge from notes
void MyGL::splitHalfEdge(HalfEdge * he1, Vertex* midPoint,
                         vector<uPtr<HalfEdge>>* newEdges) {
    if (he1 != nullptr) {
        HalfEdge* he2 = he1->sym;
        Vertex* v1 = he1->vertex;
        Vertex* v2 = he2->vertex;

        // split half edge and reassign things
        uPtr<HalfEdge> he1Sym = mkU<HalfEdge>();
        he1Sym->setVertex(v1);
        he1Sym->setFace(he1->face);
        he1Sym->next = he1->next;
        he1->next = he1Sym.get();
        he1->vertex = midPoint;

        uPtr<HalfEdge> he2Sym = mkU<HalfEdge>();
        he2Sym->setVertex(v2);
        he2Sym->setFace(he2->face);
        he2Sym->next = he2->next;
        he2->next = he2Sym.get();
        he2->vertex = midPoint;

        // set sym
        he1->sym = he2Sym.get();
        he2Sym->sym = he1;
        he2->sym = he1Sym.get();
        he1Sym->sym = he2;

        // add to newEdges
        newEdges->push_back(std::move(he1Sym));
        newEdges->push_back(std::move(he2Sym));
    }
}

void MyGL::makeTempHalfEdge(Face* f, vector<HalfEdge*> *temp) {
    HalfEdge *start = f->halfEdge;
    HalfEdge *curr = start;
    do {
        temp->push_back(curr);
        curr = curr->next->next;
    } while (curr != start);
}

// helper to map edges with their prev edge
void MyGL::makePrevEdgeMap(map<HalfEdge*, HalfEdge*> *prevEdgeMap, vector<HalfEdge*> *tempHalfEdge) {
    HalfEdge *next = tempHalfEdge->at(tempHalfEdge->size() - 1)->next;
    for (HalfEdge *prev: *tempHalfEdge) {
        prevEdgeMap->insert(make_pair(prev, next));
        next = prev->next;
    }
}

// helper to assign ring of half edges to a face
void MyGL::assignFace(vector<HalfEdge *> *tempHalfEdge, unsigned long index, Face *f) {
    tempHalfEdge->at(index)->setFace(f);
    tempHalfEdge->at(index)->next->setFace(f);
    tempHalfEdge->at(index)->next->next->setFace(f);
    tempHalfEdge->at(index)->next->next->next->setFace(f);
}

// import OBJ
void MyGL::slot_importOBJ() {
    vector<vec3> v; // verts
    vector<vec2> vt; // texture coords
    vector<vec3> vn; // normals
    map<pair<GLuint, GLuint>, HalfEdge*> symMap; // store sym edges

    // VBO stuff
    vector<vec4> pos;
    vector<vec2> uv;
    vector<vec4> nor;
    vector<GLuint> indices;

    // open file for readin'
    QString fileName = QFileDialog::getOpenFileName(0, tr("Import OBJ"),
                                                    QDir::currentPath().append(QString("../..")),  QString("*.obj"));
    QFile file(fileName);
    if (file.exists()) {
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            // get rid of old model
            m_mesh.faceList.clear();
            m_mesh.vertexList.clear();
            m_mesh.halfEdgeList.clear();

            Mesh::resetCount(); // reset count to 0

            while (!file.atEnd()) {
                QString line = file.readLine(); // read line
                QStringList splitLine = line.split(QRegularExpression(" ")); // split by word/number? chunks

                if (splitLine.at(0).compare("v", Qt::CaseInsensitive) == 0) {
                    m_mesh.vertexList.push_back(mkU<Vertex>());
                    vec3 position = vec3(splitLine.at(1).toFloat(),
                                         splitLine.at(2).toFloat(),
                                         splitLine.at(3).toFloat());
                    m_mesh.vertexList.at(m_mesh.vertexList.size() - 1)->position = position;
                } else if (splitLine.at(0).compare("vt", Qt::CaseInsensitive) == 0) {
                    vt.push_back(vec2(splitLine.at(1).toFloat(),
                                      splitLine.at(2).toFloat()));
                } else if (splitLine.at(0).compare("vn", Qt::CaseInsensitive) == 0) {
                    vn.push_back(vec3(splitLine.at(1).toFloat(),
                                      splitLine.at(2).toFloat(),
                                      splitLine.at(3).toFloat()));
                } else if (splitLine.at(0).compare("f", Qt::CaseInsensitive) == 0) {
                    // Create a face
                    uPtr<Face> face = mkU<Face>();
                    float r = (float) rand() / RAND_MAX;
                    float g = (float) rand() / RAND_MAX;
                    float b = (float) rand() / RAND_MAX;
                    face->color = vec3(r, g, b);

                    for (int i = 1; i < splitLine.length(); i++) {
                        // make half edges
                        uPtr<HalfEdge> halfEdge = mkU<HalfEdge>();
                        halfEdge->setFace(face.get());
                        // set vert, format: f pos/uv/norm
                        QStringList posUVnorm = splitLine.at(i).split("/");
                        int position = posUVnorm.at(0).toInt() - 1;
                        halfEdge->setVertex(m_mesh.vertexList.at(position).get());
                        m_mesh.halfEdgeList.push_back(std::move(halfEdge));
                    }

                    for (int i = 1; i < splitLine.length(); i++) {
                        int j = splitLine.length() - i;

                        // set next
                        if (i != splitLine.length() - 1) { // handle edge case after
                            m_mesh.halfEdgeList.at(m_mesh.halfEdgeList.size() - j)->next =
                                    m_mesh.halfEdgeList.at(m_mesh.halfEdgeList.size() - j + 1).get();
                        }

                        // set sym
                        HalfEdge* halfEdge = m_mesh.halfEdgeList.at(m_mesh.halfEdgeList.size() - j).get();
                        QStringList curr;
                        QStringList prev;
                        if (i == 1) { // wraparound edge case
                            curr = splitLine.at(1).split("/");
                            prev = splitLine.at(splitLine.size() - 1).split("/");
                        } else {
                            curr = splitLine.at(i).split("/");
                            prev = splitLine.at(i - 1).split("/");
                        }

                        pair index = make_pair(curr.at(0).toInt(), prev.at(0).toInt());
                        pair symIndex = make_pair(index.second, index.first);

                        if (symMap.find(symIndex) == symMap.end()) {
                            symMap.insert(make_pair(index, halfEdge));
                        } else {
                            halfEdge->sym = symMap.find(symIndex)->second;
                            symMap.find(symIndex)->second->sym = halfEdge;
                        }
                    }
                    // wraparound edge case
                    m_mesh.halfEdgeList.at(m_mesh.halfEdgeList.size() - 1)->next =
                            m_mesh.halfEdgeList.at(m_mesh.halfEdgeList.size() - splitLine.length() + 1).get();
                    m_mesh.faceList.push_back(std::move(face));
                }
            }
            file.close();

            m_vertexDisplay.destroy();
            m_faceDisplay.destroy();
            m_halfEdgeDisplay.destroy();

            // send data
            m_mesh.destroy();
            m_mesh.create();
            update();
            emit sig_sendMesh(&m_mesh);
        }
    }
}

// load json skeleton
void MyGL::slot_loadJSON() {
    vector<uPtr<Joint>> jointList;

    // open file for readin'
    QString filename = QFileDialog::getOpenFileName(0, tr("Load JSON"),
                                                    QDir::currentPath().append(QString("../..")),
                                                    QString("*.json"));

    QFile file(filename);
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            Joint::resetJointID(); // reset id to 0
            m_skeleton.representedJoint = -1; // reset highlight

            QString text = file.readAll();

            // json readin' business
            QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
            QJsonObject obj = doc.object();
            QJsonObject root = obj.value(QString("root")).toObject();

            queue<pair<QJsonObject, Joint*>> queue;
            queue.push(pair<QJsonObject, Joint*>(root, nullptr));
            while (!queue.empty()) {
                pair<QJsonObject, Joint*> pair = queue.front();
                QJsonObject jointObj = pair.first;
                Joint* parent = pair.second;
                queue.pop();

                // get dat infomation
                QString name = jointObj["name"].toString();
                QJsonArray position = jointObj["pos"].toArray();
                QJsonArray rotation = jointObj["rot"].toArray();
                QJsonArray children = jointObj["children"].toArray();

                vec3 pos = vec3(position[0].toDouble(),
                                position[1].toDouble(),
                                position[2].toDouble());
                vec3 axis = vec3(rotation[1].toDouble(),
                                 rotation[2].toDouble(),
                                 rotation[3].toDouble());

                quat rot = angleAxis(float(rotation[0].toDouble()), axis);

                // load that info boi
                uPtr<Joint> joint = mkU<Joint>(name, parent, pos, rot);
                if (parent != nullptr) {
                    joint->parent->children.push_back(joint.get());
                    joint->parent->addChild(joint.get());
                }

                // hit up them kids
                for (int i = 0; i < children.size(); i++) {
                    QJsonObject child = children[i].toObject();
                    queue.push(std::pair<QJsonObject, Joint*>(child, joint.get()));
                }

                jointList.push_back(std::move(joint));
            }
            m_skeleton.joints = std::move(jointList);
        }
    }
    file.close();

    // send data
    m_skeleton.destroy();
    m_skeleton.create();
    update();
    emit sig_sendSkeleton(&m_skeleton);
}

// skinning to bind skeleton to obj
void MyGL::slot_skinMesh() {
        assignBindMatrix();
        assignTranslateMatrix();

        for (uPtr<Vertex> &v : m_mesh.vertexList) {
            Joint* joint1 = nullptr;
            Joint* joint2 = nullptr;
            float joint1dist = 0;
            float joint2dist = 0;
            float joint1weight = 0;
            float joint2weight = 0;

            for (uPtr<Joint> &j : m_skeleton.joints) {
                vec3 jointPosition = vec3(j->getOverallTransformation() * vec4(0.f, 0.f, 0.f, 1.f));
                float dist = std::sqrt(pow(jointPosition[0] - v->position[0], 2)
                                     + pow(jointPosition[1] - v->position[1], 2)
                                     + pow(jointPosition[2] - v->position[2], 2));

                if (joint1) {
                    if (joint1dist > dist) {
                        joint2 = joint1;
                        joint2dist = joint1dist;
                        joint1 = j.get();
                        joint1dist = dist;
                        continue;
                    }
                } else {
                    joint1 = j.get();
                    joint1dist = dist;
                    continue;
                }

                if (joint2) {
                    if (joint2dist > dist) {
                        joint2 = j.get();
                        joint2dist = dist;
                        continue;
                    }
                } else {
                    joint2 = j.get();
                    joint2dist = dist;
                    continue;
                }
            }

            joint1weight = 1 - joint1dist / (joint1dist + joint2dist);
            joint2weight = 1 - joint2dist / (joint1dist + joint2dist);

            v->jointInfluences.push_back({joint1->id, joint1weight});
            v->jointInfluences.push_back({joint2->id, joint2weight});
        }

        vector<mat4> bind;
        vector<mat4> trans;

        for (uPtr<Joint> &j :  m_skeleton.joints) {
            bind.push_back(j->bind_matrix);
            trans.push_back(j->getOverallTransformation());
        }

        m_progSkeleton.setBindMatrix(bind);
        m_progSkeleton.setTransMatrix(trans);

        // send data
        m_skeleton.destroy();
        m_skeleton.create();

        m_mesh.destroy();
        m_mesh.create();

        update();
}

void MyGL::assignTranslateMatrix() {
    vector<mat4> translate;
    for (unsigned int i = 0; i < m_skeleton.joints.size(); i++) {
        Joint* joint = m_skeleton.joints[i].get();
        translate.push_back(joint->getOverallTransformation());
    }
    m_progSkeleton.setTransMatrix(translate);

}

void MyGL::assignBindMatrix() {
    for (unsigned int i = 0; i < m_skeleton.joints.size(); i++) {
        Joint* joint = m_skeleton.joints[i].get();
        joint->bind_matrix = inverse(joint->getOverallTransformation());
    }
}
