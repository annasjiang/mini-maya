/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    MyGL *mygl;
    QListWidget *vertsListWidget;
    QListWidget *halfEdgesListWidget;
    QListWidget *facesListWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *vertPosXSpinBox;
    QDoubleSpinBox *vertPosYSpinBox;
    QDoubleSpinBox *vertPosZSpinBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *faceBlueSpinBox;
    QLabel *label_10;
    QDoubleSpinBox *faceGreenSpinBox;
    QDoubleSpinBox *faceRedSpinBox;
    QLabel *label_11;
    QPushButton *buttonAddVert;
    QPushButton *buttonTriangulate;
    QPushButton *buttonSubdivide;
    QPushButton *buttonImportOBJ;
    QPushButton *buttonLoadJSON;
    QPushButton *buttonSkinMesh;
    QTreeWidget *treeWidget;
    QDoubleSpinBox *jointPosZSpinBox;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QDoubleSpinBox *jointPosXSpinBox;
    QDoubleSpinBox *jointPosYSpinBox;
    QLabel *label_16;
    QPushButton *buttonRotXPos;
    QPushButton *buttonRotXNeg;
    QPushButton *buttonRotYPos;
    QPushButton *buttonRotYNeg;
    QPushButton *buttonRotZPos;
    QPushButton *buttonRotZNeg;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1254, 472);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QString::fromUtf8("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QString::fromUtf8("mygl"));
        mygl->setGeometry(QRect(11, 11, 618, 432));
        vertsListWidget = new QListWidget(centralWidget);
        vertsListWidget->setObjectName(QString::fromUtf8("vertsListWidget"));
        vertsListWidget->setGeometry(QRect(640, 10, 111, 181));
        halfEdgesListWidget = new QListWidget(centralWidget);
        halfEdgesListWidget->setObjectName(QString::fromUtf8("halfEdgesListWidget"));
        halfEdgesListWidget->setGeometry(QRect(770, 10, 111, 181));
        facesListWidget = new QListWidget(centralWidget);
        facesListWidget->setObjectName(QString::fromUtf8("facesListWidget"));
        facesListWidget->setGeometry(QRect(900, 10, 111, 181));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(640, 190, 111, 16));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(770, 190, 111, 16));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(900, 190, 111, 16));
        label_3->setAlignment(Qt::AlignCenter);
        vertPosXSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosXSpinBox->setObjectName(QString::fromUtf8("vertPosXSpinBox"));
        vertPosXSpinBox->setGeometry(QRect(770, 230, 62, 22));
        vertPosXSpinBox->setMinimum(-99.989999999999995);
        vertPosYSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosYSpinBox->setObjectName(QString::fromUtf8("vertPosYSpinBox"));
        vertPosYSpinBox->setGeometry(QRect(850, 230, 62, 22));
        vertPosYSpinBox->setMinimum(-99.989999999999995);
        vertPosZSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosZSpinBox->setObjectName(QString::fromUtf8("vertPosZSpinBox"));
        vertPosZSpinBox->setGeometry(QRect(930, 230, 62, 22));
        vertPosZSpinBox->setMinimum(-99.989999999999995);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(660, 230, 101, 21));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(760, 250, 61, 16));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(840, 250, 61, 16));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(920, 250, 61, 16));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(920, 300, 61, 16));
        label_8->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(760, 300, 61, 16));
        label_9->setAlignment(Qt::AlignCenter);
        faceBlueSpinBox = new QDoubleSpinBox(centralWidget);
        faceBlueSpinBox->setObjectName(QString::fromUtf8("faceBlueSpinBox"));
        faceBlueSpinBox->setGeometry(QRect(930, 280, 62, 22));
        faceBlueSpinBox->setMaximum(1.000000000000000);
        faceBlueSpinBox->setSingleStep(0.050000000000000);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(660, 280, 71, 21));
        faceGreenSpinBox = new QDoubleSpinBox(centralWidget);
        faceGreenSpinBox->setObjectName(QString::fromUtf8("faceGreenSpinBox"));
        faceGreenSpinBox->setGeometry(QRect(850, 280, 62, 22));
        faceGreenSpinBox->setMaximum(1.000000000000000);
        faceGreenSpinBox->setSingleStep(0.050000000000000);
        faceRedSpinBox = new QDoubleSpinBox(centralWidget);
        faceRedSpinBox->setObjectName(QString::fromUtf8("faceRedSpinBox"));
        faceRedSpinBox->setGeometry(QRect(770, 280, 62, 22));
        faceRedSpinBox->setMaximum(1.000000000000000);
        faceRedSpinBox->setSingleStep(0.050000000000000);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(840, 300, 61, 16));
        label_11->setAlignment(Qt::AlignCenter);
        buttonAddVert = new QPushButton(centralWidget);
        buttonAddVert->setObjectName(QString::fromUtf8("buttonAddVert"));
        buttonAddVert->setGeometry(QRect(1030, 210, 211, 32));
        buttonTriangulate = new QPushButton(centralWidget);
        buttonTriangulate->setObjectName(QString::fromUtf8("buttonTriangulate"));
        buttonTriangulate->setGeometry(QRect(1030, 250, 211, 32));
        buttonSubdivide = new QPushButton(centralWidget);
        buttonSubdivide->setObjectName(QString::fromUtf8("buttonSubdivide"));
        buttonSubdivide->setGeometry(QRect(1030, 290, 211, 32));
        buttonImportOBJ = new QPushButton(centralWidget);
        buttonImportOBJ->setObjectName(QString::fromUtf8("buttonImportOBJ"));
        buttonImportOBJ->setGeometry(QRect(1030, 330, 211, 32));
        buttonLoadJSON = new QPushButton(centralWidget);
        buttonLoadJSON->setObjectName(QString::fromUtf8("buttonLoadJSON"));
        buttonLoadJSON->setGeometry(QRect(1030, 370, 211, 32));
        buttonSkinMesh = new QPushButton(centralWidget);
        buttonSkinMesh->setObjectName(QString::fromUtf8("buttonSkinMesh"));
        buttonSkinMesh->setGeometry(QRect(1030, 410, 211, 32));
        treeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(1030, 10, 211, 181));
        jointPosZSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosZSpinBox->setObjectName(QString::fromUtf8("jointPosZSpinBox"));
        jointPosZSpinBox->setGeometry(QRect(930, 330, 62, 22));
        jointPosZSpinBox->setMinimum(-99.989999999999995);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(760, 350, 61, 16));
        label_12->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(660, 330, 101, 21));
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(840, 350, 61, 16));
        label_14->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(920, 350, 61, 16));
        label_15->setAlignment(Qt::AlignCenter);
        jointPosXSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosXSpinBox->setObjectName(QString::fromUtf8("jointPosXSpinBox"));
        jointPosXSpinBox->setGeometry(QRect(770, 330, 62, 22));
        jointPosXSpinBox->setMinimum(-99.989999999999995);
        jointPosYSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosYSpinBox->setObjectName(QString::fromUtf8("jointPosYSpinBox"));
        jointPosYSpinBox->setGeometry(QRect(850, 330, 62, 22));
        jointPosYSpinBox->setMinimum(-99.989999999999995);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(660, 380, 101, 21));
        buttonRotXPos = new QPushButton(centralWidget);
        buttonRotXPos->setObjectName(QString::fromUtf8("buttonRotXPos"));
        buttonRotXPos->setGeometry(QRect(760, 380, 71, 32));
        buttonRotXNeg = new QPushButton(centralWidget);
        buttonRotXNeg->setObjectName(QString::fromUtf8("buttonRotXNeg"));
        buttonRotXNeg->setGeometry(QRect(760, 410, 71, 32));
        buttonRotYPos = new QPushButton(centralWidget);
        buttonRotYPos->setObjectName(QString::fromUtf8("buttonRotYPos"));
        buttonRotYPos->setGeometry(QRect(840, 380, 71, 32));
        buttonRotYNeg = new QPushButton(centralWidget);
        buttonRotYNeg->setObjectName(QString::fromUtf8("buttonRotYNeg"));
        buttonRotYNeg->setGeometry(QRect(840, 410, 71, 32));
        buttonRotZPos = new QPushButton(centralWidget);
        buttonRotZPos->setObjectName(QString::fromUtf8("buttonRotZPos"));
        buttonRotZPos->setGeometry(QRect(920, 380, 71, 32));
        buttonRotZNeg = new QPushButton(centralWidget);
        buttonRotZNeg->setObjectName(QString::fromUtf8("buttonRotZNeg"));
        buttonRotZNeg->setGeometry(QRect(920, 410, 71, 32));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1254, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "OpenGLDemo", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionCamera_Controls->setText(QApplication::translate("MainWindow", "Camera Controls", nullptr));
        label->setText(QApplication::translate("MainWindow", "Vertices", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Half-Edges", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Faces", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Vertex Position", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "X", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Y", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Z", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Blue", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Red", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Face Color", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Green", nullptr));
        buttonAddVert->setText(QApplication::translate("MainWindow", "add vertex", nullptr));
        buttonTriangulate->setText(QApplication::translate("MainWindow", "triangulate", nullptr));
        buttonSubdivide->setText(QApplication::translate("MainWindow", "subdivide", nullptr));
        buttonImportOBJ->setText(QApplication::translate("MainWindow", "import obj", nullptr));
        buttonLoadJSON->setText(QApplication::translate("MainWindow", "load json", nullptr));
        buttonSkinMesh->setText(QApplication::translate("MainWindow", "skin mesh", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "X", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "Joint Position", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "Y", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "Z", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "Joint Rotation", nullptr));
        buttonRotXPos->setText(QApplication::translate("MainWindow", "+x", nullptr));
        buttonRotXNeg->setText(QApplication::translate("MainWindow", "-x", nullptr));
        buttonRotYPos->setText(QApplication::translate("MainWindow", "+y", nullptr));
        buttonRotYNeg->setText(QApplication::translate("MainWindow", "-y", nullptr));
        buttonRotZPos->setText(QApplication::translate("MainWindow", "+z", nullptr));
        buttonRotZNeg->setText(QApplication::translate("MainWindow", "-z", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
