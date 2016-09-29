#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {

    setWindowTitle(tr("Something Title"));
    view = new MyQGraphicsView;//creating grafic scence inside of main window
    this->setCentralWidget(view);
    go = new QPushButton("start", this);
    this->setMenuWidget(go);
    //connect(go, SIGNAL (released()), this, SLOT (start()));

    // основа рассчета
    timer = new QTimer;
    connect(timer, SIGNAL (timeout()), this, SLOT (start()));
    timer->start(100);

    //
}

void MainWindow::start() {
    view->clear();
    view->draw();
}

MainWindow::~MainWindow() {
}
