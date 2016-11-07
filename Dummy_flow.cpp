//
// Created by nikita on 29.09.16.
//

#include "Dummy_flow.h"

void Dummy_flow::test_W(){
    std::ofstream outfile("/home/nikita/ClionProjects/dummy_sph/python/check_W.txt");
    outfile << "begin.\n";
    double x = 0;
    while(x < 2){
        double value = dW_functions::dW_cubic_smoothing_function(x, 1);
        outfile << x << " "<< value << '\n';
        x+=0.01;
    }
    outfile << "end.\n";
}

Dummy_flow::Dummy_flow() {
    test_W();
    init_shock_tube_book();
}

void Dummy_flow::init_Bykov(){
    left = 0;
    right = 1;

    bigPN = 80;

    ghoustN = 15;

    N = 130;

    begin = ghoustN;
    end = N - ghoustN;

    double P = 1;
    double p = 1;
    double e = 2.5;
    double mass = 1.0 / 160.0;
    for (int i = 0; i < bigPN + ghoustN; ++i) {
        Particle a(0, p, P, e, 0, 0, mass);
        data.push_back(a);
    }
    //низкое давление
    P = 0.1795;
    p = 0.25;
    e = 1.795;
    for (int i = bigPN + ghoustN; i < N; ++i) {
        Particle a(0, p, P, e, 0, 0, mass);
        data.push_back(a);
    }
    //позиция для заряженых
    double step1 = 0.5 / bigPN;
    for (int i = 0; i < bigPN + ghoustN; ++i) {
        double cur_x = left + step1 * i - ghoustN * step1;
        data[i].set_pos(cur_x, 0);
    }
    double last_x = left + step1 * (bigPN + ghoustN) - ghoustN * step1;
    //для незаряженных
    double step2 = 0.5 / (N - bigPN - 2 * ghoustN);
    for (int i = 0; i < N - bigPN - ghoustN; ++i) {
        double cur_x = last_x + step2 + step2 * i;
        data[bigPN + ghoustN + i].set_pos(cur_x, 0);
    }
}

void Dummy_flow::init_shock_tube_book(){
    left = -0.6;
    right = 0.6;

    bigPN = 320;

    ghoustN = 20;

    N = 440;

    begin = ghoustN;
    end = N - ghoustN;

    double P = 1;
    double p = 1;
    double e = 2.5;
    double mass = 1.0 / 160.0;
    for (int i = 0; i < bigPN + ghoustN; ++i) {
        Particle a(0, p, P, e, 0, 0, mass);
        data.push_back(a);
    }
    //низкое давление
    P = 0.1795;
    p = 0.25;
    e = 1.795;
    for (int i = bigPN + ghoustN; i < N; ++i) {
        Particle a(0, p, P, e, 0, 0, mass);
        data.push_back(a);
    }
    //позиция для заряженых
    double step1 = 0.6 / bigPN;
    for (int i = 0; i < bigPN + ghoustN; ++i) {
        double cur_x = left + step1 * i - ghoustN * step1;
        data[i].set_pos(cur_x, 0);
    }
    //для незаряженных
    double step2 = 0.6 / (N - bigPN - 2 * ghoustN);
    double last_x = left + step2 * (bigPN + ghoustN) - ghoustN * step1;
    for (int i = 0; i < N - bigPN - ghoustN; ++i) {
        double cur_x = last_x + step2 * i;
        data[bigPN + ghoustN + i].set_pos(cur_x, 0);
    }
}


//функция расчета производный
void Dummy_flow::calculate_dir() {

    for (int i = begin; i < end; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i != j) {
                data[i].derivatives_from_particle(data[j],i, j, iteration);
            }
        }
    }
}

void Dummy_flow::calculate_final() {
    double cur_maxV = 0;
    double cur_maxC = 0;
    double cur_maxP = 0;
    double cur_minP = 100500;

    for (int i = ghoustN; i < N - ghoustN; ++i) {
        try {
            data[i].ronge_kutt(dt);
        }
        catch (Particle a) {
            assert(iteration < 0);
        }

        cur_maxV = data[i].V();
        cur_maxC = data[i].C();
        cur_maxP = data[i].P();
        cur_minP = data[i].P();
        if (cur_maxV > max_V) max_V = cur_maxV;
        if (cur_maxC > max_C) max_C = cur_maxC;
        if (cur_maxP > max_P) max_P = cur_maxP;
        if (cur_minP < min_P) min_P = cur_minP;
    }

}


//Рассчитываются производные, а затем финальные значения
void Dummy_flow::calculate() {
    calculate_dir();
    calculate_final();
    curent_time += dt;
    iteration++;
}