#include "myqgraphicsview.h"

//from QT
MyQGraphicsView::MyQGraphicsView(QWidget *parent) :
        QGraphicsView(parent) {
    //boudCounter=0;
    scene = new QGraphicsScene();
    //Исключительно для дебага
    //
    this->setScene(scene);
    flow = new Dummy_flow_drawer();
    //}
}

//
MyQGraphicsView::~MyQGraphicsView() {
    delete flow;
    delete scene;
}

//
void MyQGraphicsView::clear() {
    scene->clear();
}
//

void MyQGraphicsView::mousePressEvent(QMouseEvent *e) {
    //flow->calculate(scene);

}

//
void MyQGraphicsView::mouseDoubleClickEvent(QMouseEvent *e) {

}

//
void MyQGraphicsView::draw() {

    if (flow->time() < 200000) {
        flow->calculate_step(scene);
    }
}

//
void MyQGraphicsView::ClearWindow() {
    scene->clear();
}
