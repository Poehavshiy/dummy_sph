//
// Created by nikita on 29.09.16.
//

#include "Dummy_flow_drawer.h"
Dummy_flow_drawer::Dummy_flow_drawer() :
        Dummy_flow() {
    cof = 1000;
    cofx = -100;
    cofy = 100;
    python_iter = 0;
    for(int i = 20; i<2000; i+=20){
        frames.push_back(i);
    }

};

Dummy_flow_drawer::COLOUR Dummy_flow_drawer::GetColour(double v, double vmin, double vmax) {
    COLOUR c = {1.0, 1.0, 1.0}; // white
    double dv;

    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
        c.r = 0;
        c.g = 4 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
        c.r = 0;
        c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
        c.r = 4 * (v - vmin - 0.5 * dv) / dv;
        c.b = 0;
    } else {
        c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
        c.b = 0;
    }

    return (c);
}



void Dummy_flow_drawer::draw_data(QGraphicsScene *scene){
    double rad = 1;
    for (int i = 0; i < data.size(); ++i) {
        const Particle& curent = data[i];
        double value = data[i].P();
        COLOUR currentC = GetColour(value, 0, max_P);
        QColor QTcurrentC;
        QTcurrentC.setRgb(255 * currentC.r, 255 * currentC.g, 255 * currentC.b);
        scene->addEllipse( (curent.X() ) * cof - rad + cofx, cofy, rad * 2.0, rad * 2.0,
                          QTcurrentC, QBrush(QTcurrentC));
    }
}

void Dummy_flow_drawer::set_text(QString &&text, QGraphicsScene *scene, pair<int, int> &&position, double &&value) {
    QString test_to_show = text + QString::number(value);
    QGraphicsTextItem *text_item = scene->addText(test_to_show);
    text_item->setPos(position.first, position.second);
}


void Dummy_flow_drawer::show_information(QGraphicsScene *scene) {
    set_text("iteration=", scene, pair<int, int>(-50, -150), std::move(iteration));
    set_text("time=", scene, pair<int, int>(-50, -130), std::move(curent_time));
    set_text("maxP=", scene, pair<int, int>(-50, -110), std::move(max_P));
    set_text("minP=", scene, pair<int, int>(-50, -90), std::move(min_P));
    set_text("maxV=", scene, pair<int, int>(50, -150), std::move(max_V));
}

void Dummy_flow_drawer::write_py_data(int target_itertion) {
    std::ofstream outfile(mathplot_path + "mathplot"+ to_string(target_itertion) + ".txt");
    outfile << "begin.\n";
    for (int i = 0; i < data.size(); ++i) {
        double x = data[i].X();
        //double y = data[i].Y();
        double value = data[i].P();
        outfile << x << " "<< value << '\n';
    }
    outfile << "end.\n";
}

void Dummy_flow_drawer::calculate_step(QGraphicsScene *scene) {
    calculate();
    draw_data(scene);
    show_information(scene);
    if (iteration == frames[python_iter]) {
        ++python_iter;
        write_py_data(python_iter);
    }
}