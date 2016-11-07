//
// Created by nikita on 29.09.16.
//

#ifndef DUMMY_SPH_DUMMY_FLOW_H
#define DUMMY_SPH_DUMMY_FLOW_H


#include "Particile.h"

class Dummy_flow {
protected:
    //массив всех частиц
    vector<Particle> data;

    int iteration = 0;

    double curent_time = 0;

    double dt = 0.001;

    double h;

    double max_V = 0;
    double max_C = 0;
    //всякие информативные штуки
    double max_P = 0;
    double min_P = 100500;

    double left , right;

    int bigPN;

    int ghoustN ;

    int N ;

    int begin;
    int end;

    void calculate_dir();

    void calculate_final();

    void init_Bykov();

    void init_shock_tube_book();

    void test_W();
public:
    //конструктор, тут инициализируются начальные параметры потока
    Dummy_flow();

    double  time(){
        return curent_time;
    }

    void calculate();

};


#endif //DUMMY_SPH_DUMMY_FLOW_H
