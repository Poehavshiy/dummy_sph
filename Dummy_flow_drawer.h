//
// Created by nikita on 29.09.16.
//

#ifndef DUMMY_SPH_DUMMY_FLOW_DRAWER_H
#define DUMMY_SPH_DUMMY_FLOW_DRAWER_H

#include "Dummy_flow.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QPointF>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenu>
#include <QtGui>
#include <QMenuBar>
#include <QToolButton>
#include <QtWidgets>
//
class Dummy_flow_drawer: public Dummy_flow {
    typedef struct {
        double r,g,b;
    } COLOUR;

    int cof ;
    int cofx ;
    int cofy ;
    int python_iter = 1;
    //
    void draw_data(QGraphicsScene *scene);
    //
    COLOUR GetColour(double v,double vmin,double vmax);

    void show_information(QGraphicsScene *scene);

    void set_text(QString&& text, QGraphicsScene *scene,pair<int, int>&& position, double&& value = 0);

    void write_py_data(int target_itertion);

    string mathplot_path = "/home/nikita/ClionProjects/dummy_sph/python/";

    vector<int> frames;

public:

    Dummy_flow_drawer();

    virtual void calculate_step(QGraphicsScene *scene) ;
};


#endif //DUMMY_SPH_DUMMY_FLOW_DRAWER_H
