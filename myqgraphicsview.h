#ifndef MyQGraphicsView_H
#define MyQGraphicsView_H

#include "Dummy_flow_drawer.h"

class MyQGraphicsView : public QGraphicsView {
Q_OBJECT
public:
    explicit MyQGraphicsView(QWidget *parent = 0);

    ~MyQGraphicsView();

signals:

public slots:

    //drawing
    void mousePressEvent(QMouseEvent *e);

    void ClearWindow();

    void mouseDoubleClickEvent(QMouseEvent *e);

private:
    QGraphicsScene *scene;

    Dummy_flow_drawer *flow;
    //
    int counter_help = 0;
public:
    void draw();

    void clear();


};

#endif
