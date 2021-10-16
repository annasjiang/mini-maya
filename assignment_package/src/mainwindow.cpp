#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    // init widgets
    connect(ui->mygl, SIGNAL(sig_sendMesh(Mesh*)), this, SLOT(slot_receiveMesh(Mesh*)));
    connect(ui->mygl, SIGNAL(sig_sendSkeleton(Skeleton*)), this, SLOT(slot_receiveSkeleton(Skeleton*)));

    // selected item
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_select(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_select(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(slot_select(QListWidgetItem*)));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_selectJoint(QTreeWidgetItem*, int)));

    // vert pos
    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_x(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_y(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_z(double)));

    // color
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_red(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_green(double)));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_blue(double)));

    // added buttons
    connect(ui->buttonAddVert, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_addVert()));
    connect(ui->buttonTriangulate, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_triangulate()));
    connect(ui->buttonSubdivide, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_subdivide()));
    connect(ui->buttonImportOBJ, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_importOBJ()));
    connect(ui->buttonLoadJSON, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_loadJSON()));
    connect(ui->buttonSkinMesh, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_skinMesh()));

    // joint adjustments
    connect(ui->jointPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_jointPosX(double)));
    connect(ui->jointPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_jointPosY(double)));
    connect(ui->jointPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_jointPosZ(double)));

    connect(ui->buttonRotXPos, SIGNAL(clicked(bool)), this, SLOT(slot_jointRotXPos()));
    connect(ui->buttonRotXNeg, SIGNAL(clicked(bool)), this, SLOT(slot_jointRotXNeg()));
    connect(ui->buttonRotYPos, SIGNAL(clicked(bool)), this, SLOT(slot_jointRotYPos()));
    connect(ui->buttonRotYNeg, SIGNAL(clicked(bool)), this, SLOT(slot_jointRotYNeg()));
    connect(ui->buttonRotZPos, SIGNAL(clicked(bool)), this, SLOT(slot_jointRotZPos()));
    connect(ui->buttonRotZNeg, SIGNAL(clicked(bool)), this, SLOT(slot_jointRotZNeg()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::slot_receiveMesh(Mesh* m) {
    for (const uPtr<Face> &f : m->faceList) {
        ui->facesListWidget->addItem(f.get());
    }
    for (const uPtr<HalfEdge> &he : m->halfEdgeList) {
        ui->halfEdgesListWidget->addItem(he.get());
    }
    for (const uPtr<Vertex> &v : m->vertexList) {
        ui->vertsListWidget->addItem(v.get());
    }
}

void MainWindow::slot_receiveSkeleton(Skeleton *skeleton) {
    for (const uPtr<Joint> &j : skeleton->joints) {
        ui->treeWidget->addTopLevelItem(j.get());
    }
}

// joint business
void MainWindow::slot_selectJoint(QTreeWidgetItem* j, int n) {
    if (j) {
        Joint* joint = dynamic_cast<Joint*>(j);
        if (joint) {
            ui->mygl->m_skeleton.updateJoint(joint);
            selectedJoint = joint;

            ui->jointPosXSpinBox->setValue(joint->position[0]);
            ui->jointPosYSpinBox->setValue(joint->position[1]);
            ui->jointPosZSpinBox->setValue(joint->position[2]);

            ui->mygl->m_skeleton.destroy();
            ui->mygl->m_skeleton.create();
            ui->mygl->update();
            currentTransformationText(joint);
        }
    }
}

// position
void MainWindow::slot_jointPosX(double x) {
    if (selectedJoint != nullptr) {
            selectedJoint =  ui->mygl->m_skeleton.joints[ui->mygl->m_skeleton.representedJoint].get();
            selectedJoint->position.x = x;
            ui->mygl->assignTranslateMatrix();

            ui->mygl->m_skeleton.destroy();
            ui->mygl->m_skeleton.create();
            ui->mygl->update();

            currentTransformationText(selectedJoint);
    }
}


void MainWindow::slot_jointPosY(double y) {
    if (selectedJoint != nullptr) {
            selectedJoint = ui->mygl->m_skeleton.joints[ui->mygl->m_skeleton.representedJoint].get();
            selectedJoint->position.y = y;
            ui->mygl->assignTranslateMatrix();

            ui->mygl->m_skeleton.destroy();
            ui->mygl->m_skeleton.create();
            ui->mygl->update();

            currentTransformationText(selectedJoint);
    }
}

void MainWindow::slot_jointPosZ(double z) {
    if (selectedJoint != nullptr) {
            selectedJoint = ui->mygl->m_skeleton.joints[ui->mygl->m_skeleton.representedJoint].get();
            selectedJoint->position.z = z;
            ui->mygl->assignTranslateMatrix();

            ui->mygl->m_skeleton.destroy();
            ui->mygl->m_skeleton.create();
            ui->mygl->update();

            currentTransformationText(selectedJoint);
    }
}

// rotation
void MainWindow::slot_jointRotXPos() {
    if (selectedJoint != nullptr) {
        vec3 angles = eulerAngles(selectedJoint->rotation);
        quat x = angleAxis(radians(5.f), vec3(1, 0, 0));
        quat y = angleAxis(radians(angles[1]), vec3(0, 1, 0));
        quat z = angleAxis(radians(angles[2]), vec3(0, 0, 1));
        selectedJoint->rotation *= (x * y * z);
        ui->mygl->assignTranslateMatrix();

        ui->mygl->m_skeleton.destroy();
        ui->mygl->m_skeleton.create();
        ui->mygl->update();

        currentTransformationText(selectedJoint);
    }
}

void MainWindow::slot_jointRotXNeg() {
    if (selectedJoint != nullptr) {
        vec3 angles = eulerAngles(selectedJoint->rotation);
        quat x = angleAxis(radians(-5.f), vec3(1, 0, 0));
        quat y = angleAxis(radians(angles[1]), vec3(0, 1, 0));
        quat z = angleAxis(radians(angles[2]), vec3(0, 0, 1));
        selectedJoint->rotation *= (x * y * z);
        ui->mygl->assignTranslateMatrix();

        ui->mygl->m_skeleton.destroy();
        ui->mygl->m_skeleton.create();
        ui->mygl->update();

        currentTransformationText(selectedJoint);
    }
}

void MainWindow::slot_jointRotYPos() {
    if (selectedJoint != nullptr) {
        vec3 angles = eulerAngles(selectedJoint->rotation);
        quat x = angleAxis(radians(angles[0]), vec3(1, 0, 0));
        quat y = angleAxis(radians(5.f), vec3(0, 1, 0));
        quat z = angleAxis(radians(angles[2]), vec3(0, 0, 1));
        selectedJoint->rotation *= (x * y * z);
        ui->mygl->assignTranslateMatrix();

        ui->mygl->m_skeleton.destroy();
        ui->mygl->m_skeleton.create();
        ui->mygl->update();

        currentTransformationText(selectedJoint);
    }
}

void MainWindow::slot_jointRotYNeg() {
    if (selectedJoint != nullptr) {
        vec3 angles = eulerAngles(selectedJoint->rotation);
        quat x = angleAxis(radians(angles[0]), vec3(1, 0, 0));
        quat y = angleAxis(radians(-5.f), vec3(0, 1, 0));
        quat z = angleAxis(radians(angles[2]), vec3(0, 0, 1));
        selectedJoint->rotation *= (x * y * z);
        ui->mygl->assignTranslateMatrix();

        ui->mygl->m_skeleton.destroy();
        ui->mygl->m_skeleton.create();
        ui->mygl->update();

        currentTransformationText(selectedJoint);
    }
}

void MainWindow::slot_jointRotZPos() {
    if (selectedJoint != nullptr) {
        vec3 angles = glm::eulerAngles(selectedJoint->rotation);
        quat x = angleAxis(radians(angles[0]), vec3(1, 0, 0));
        quat y = angleAxis(radians(angles[1]), vec3(0, 1, 0));
        quat z = angleAxis(radians(1.f), vec3(0, 0, 1));
        selectedJoint->rotation *= (x * y * z);
        ui->mygl->assignTranslateMatrix();

        ui->mygl->m_skeleton.destroy();
        ui->mygl->m_skeleton.create();
        ui->mygl->update();

        currentTransformationText(selectedJoint);
    }
}

void MainWindow::slot_jointRotZNeg() {
    if (selectedJoint != nullptr) {
        vec3 angles = eulerAngles(selectedJoint->rotation);
        quat x = angleAxis(radians(angles[0]), vec3(1, 0, 0));
        quat y = angleAxis(radians(angles[1]), vec3(0, 1, 0));
        quat z = angleAxis(radians(-1.f), vec3(0, 0, 1));
        selectedJoint->rotation *= (x * y * z);
        ui->mygl->assignTranslateMatrix();

        ui->mygl->m_skeleton.destroy();
        ui->mygl->m_skeleton.create();
        ui->mygl->update();

        currentTransformationText(selectedJoint);
    }
}

// helper for getting the current rotation of a joint
void MainWindow::currentTransformationText(Joint* j) {
    float currPosX = degrees(j->position[0]);
    float currPosY = degrees(j->position[1]);
    float currPosZ = degrees(j->position[2]);
    QString currentPositionData = QString::fromStdString(string("Position: (").append(
                                                             std::to_string(currPosX)).append(",  ").append(
                                                             std::to_string(currPosY)).append(",  ").append(
                                                             std::to_string(currPosZ)).append(")"));
    ui->labelJointInfoPosition->setText(currentPositionData);

    float currRotX = degrees(j->rotation[0]);
    float currRotY = degrees(j->rotation[1]);
    float currRotZ = degrees(j->rotation[2]);
    QString currentRotationData = QString::fromStdString(string("Rotation: (").append(
                                                             std::to_string(currRotX)).append(",  ").append(
                                                             std::to_string(currRotY)).append(",  ").append(
                                                             std::to_string(currRotZ)).append(")"));
    ui->labelJointInfoRotate->setText(currentRotationData);
}
