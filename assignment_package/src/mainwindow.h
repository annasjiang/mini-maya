#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vertex.h>
#include <face.h>
#include <halfedge.h>
#include <mesh.h>
#include <skeleton.h>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void currentTransformationText(Joint* j);

private slots:
    void on_actionQuit_triggered();
    void on_actionCamera_Controls_triggered();

public slots:
    void slot_receiveMesh(Mesh*);
    void slot_receiveSkeleton(Skeleton*);
    void slot_selectJoint(QTreeWidgetItem*, int);

    // joint movement
    void slot_jointPosX(double);
    void slot_jointPosY(double);
    void slot_jointPosZ(double);
    void slot_jointRotXPos();
    void slot_jointRotXNeg();
    void slot_jointRotYPos();
    void slot_jointRotYNeg();
    void slot_jointRotZPos();
    void slot_jointRotZNeg();


private:
    Ui::MainWindow *ui;
    Joint *selectedJoint;
};


#endif // MAINWINDOW_H
