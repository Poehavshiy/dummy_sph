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

    int iteration;

    double dt;

    double h;

    double max_V = 0;
    double max_C = 0;
    //всякие информативные штуки
    double max_P = 0;
    double min_P = 100500;

    double left, right;

    int bigPN = 80;

    int ghoustN = 15;

    int N ;

    double grad_w_test( double x );

    double two_part_p(const Particle &a, const Particle &b );

    double two_part_v(const Particle &a, const Particle &b );

    double two_part_energy(const Particle &a, const Particle &b );

    double two_part_art_visc(const Particle &a, const Particle &b);

    double two_part_a;

    void calculate_dir();

    void calculate_final();

    double curent_time;

public:
    //конструктор, тут инициализируются начальные параметры потока
    Dummy_flow();

    double  time(){
        return curent_time;
    }

    int get_iteration(){
        return iteration;
    }

    void calculate();

};


#endif //DUMMY_SPH_DUMMY_FLOW_H
