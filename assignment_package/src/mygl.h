#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "QListWidget"
#include "mesh.h"
#include "vertexdisplay.h"
#include "facedisplay.h"
#include "halfedgedisplay.h"
#include "skeleton.h"

using namespace glm;
using namespace std;

class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

public:
    // member vars
    Mesh m_mesh;
    Skeleton m_skeleton;
    Vertex *m_vertex;
    VertexDisplay m_vertexDisplay;
    Face *m_face;
    FaceDisplay m_faceDisplay;
    HalfEdge *m_halfEdge;
    HalfEdgeDisplay m_halfEdgeDisplay;
    QListWidgetItem* m_selected;
    bool highlightMode;
    vec3 spinBoxColor;

    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // helpers for catmull clark
    void initEdges2Split(vector<HalfEdge*> *edges2Split);
    void computeCentroid(map<Face*, Vertex*> *centroidMap,
                         vector<uPtr<Vertex>> *newVerts);
    void computeMidpoints(map<Face*, Vertex*> *centroidMap,
                          map<HalfEdge*, Vertex*> *midpointMap,
                          vector<HalfEdge*> *fullEdge,
                          vector<uPtr<Vertex>> *newVerts);
    void smoothOgVerts(map<Face*, Vertex*> *centroidMap,
                       map<HalfEdge*, Vertex*> *midpointMap);
    void quadrangulate(map<Face*, Vertex*> *centroidMap,
                      map<HalfEdge*, Vertex*> *midpointMap,
                      vector<HalfEdge*> *edges2Split,
                      vector<uPtr<Vertex>> *newVerts,
                      vector<uPtr<HalfEdge>> *newEdges,
                      vector<uPtr<Face>> *newFaces);
    // helpers for quadragulate that big boi
    void splitHalfEdge(HalfEdge * he1, Vertex* midPoint,
                       vector<uPtr<HalfEdge>>* newEdges);
    void makeTempHalfEdge(Face* f, vector<HalfEdge*> *temp);
    void makePrevEdgeMap(map<HalfEdge*, HalfEdge*> *map_prev,
                         vector<HalfEdge*> *new_halfEdge);
    void assignFace(vector<HalfEdge*> *tempHalfEdge,
                    unsigned long index, Face* f);
    // helpers for skinning mesh
    void assignTranslateMatrix();
    void assignBindMatrix();

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void slot_x(double);
    void slot_y(double);
    void slot_z(double);
    void slot_red(double);
    void slot_green(double);
    void slot_blue(double);
    void slot_select(QListWidgetItem*);
    void slot_addVert();
    void slot_triangulate();
    void slot_subdivide();
    void slot_importOBJ();
    void slot_loadJSON();
    void slot_skinMesh();

signals:
    void sig_sendMesh(Mesh*);
    void sig_sendSkeleton(Skeleton*);
};

#endif // MYGL_H

