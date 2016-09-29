//
// Created by nikita on 29.09.16.
//

#include "Dummy_flow.h"
double old_kernel(double R, double h){
    double minR = 0.0000000001;
    if (R <= 0 || R > 2 || R < minR) return 0;
    double a = 1.0 / (3.14 * pow(h, 4.0));
    double result = 0;
    if (R > 0 && R <= 1) {
        result = (9.0 / 4.0) * pow(R, 2.0) - 3 * R;
    } else if (R > 1 && R <= 2) {
        double debug = 2 - R;
        result = -(3.0 / 4.0) * pow(debug, 2.0);
    }
    return result*a;
}

// производная сглаживающей функции (формула 3.81) со страницы 92
double new_quartic(double R, double h){
    if (R <= 0 || R > 2 ) return 0;
    double a = 1/h;

    double result = -(18.0 / 8.0) * R + (57.0/24.0) * pow(R,2) - (20.0/32.0)*pow(R,3);
    return result*a;
}



Dummy_flow::Dummy_flow() {
    iteration = 0;

    curent_time = 0;

    dt = 0.005;

    left = -0.6;
    right = 0.6;

    N = 400;

    h = (abs(left - right) / N) * 1;

    double step = abs(left - right) / N;

    double P = 1;
    double p = 1;
    double e = 2.5;
    double mass = 0.001875;
    for (int i = 0; i < 320; ++i) {
        Particle a(0, p, P, e, 0, 0, mass);
        data.push_back(a);
    }
    //низкое давление
    P = 0.1795;
    p = 0.25;
    e = 1.795;
    for (int i = 320; i < N; ++i) {
        Particle a(0, p, P , e , 0, 0, mass);
        data.push_back(a);
    }
    //позиция для заряженых
    double step1 = 0.001875;
    for (int i = 0; i < 320; ++i) {
        double cur_x = left + step1 * i;
        data[i].set_pos(cur_x, 0);
    }
    //для незаряженных
    double step2 = 0.0075;
    for (int i = 320; i < N; ++i) {
        double cur_x = left + step1*320 + step2 * (i-320);
        data[i].set_pos(cur_x, 0);
    }
}


//производная W собственно сама функция вычисляется в new_quartic
//здесь этот результат умножается на (xi-xj)/r -пременная фактор
//это соответствует способу вычисления производной W по формуле 2.23 со страницы 43
double Dummy_flow::grad_w_test(double x) {
    double minR = 0.0000000001;
    if(abs(x) < minR) return 0;
    double r = abs(x) / h;
    double factor = x / abs(x);
    double result = new_quartic(r,h);
    return result*factor;
}


//вычисление части производной плотности от 2 частиц, соответствует формуле
// 4.34 на странице 124
double Dummy_flow::two_part_p(const Particle &a, const Particle &b) {
    double Mj = b.M();//mj
    double delta_vx = a.Vx() - b.Vx(); //Vij в книге
    double deltaX = a.X() - b.X();//значение радиуса для градиеета W
    //скалярное произведение скоростей и градиента
    double VijWij = delta_vx * grad_w_test(deltaX);//скалярное произведение
    double res = Mj * VijWij;//ответ
    return res;
}

//вычисление части производной скорости от 2 частиц, соответствует формуле
// 4.61 на странице 124

double Dummy_flow::two_part_v(const Particle &a, const Particle &b) {
    double Mj = b.M();//mj
    double deltaX = a.X() - b.X();
    double Wij = grad_w_test(deltaX);
    //значение в скобках
    double brackets1 = -( a.P() / pow(a.p(), 2) + b.P() / pow(b.p(), 2) );
    //
    double res = Mj * brackets1 * Wij;//
    return res;
}

//вычисление части производной энергии от 2 частиц, соответствует формуле
// 4.63 на странице 124
double Dummy_flow::two_part_energy(const Particle &a, const Particle &b) {
    double Mj = b.M();
    double delta_vx = a.Vx() - b.Vx();
    double deltaX = a.X() - b.X();

    //скалярное произведение Vij и Wij
    double VijWij = delta_vx * grad_w_test(deltaX);
    //
    double brackets1 = (a.P() / pow(a.p(), 2) + b.P() / pow(b.p(), 2));
    //

    double res = 0.5 * VijWij * Mj * brackets1;

    return res;
}


//функция расчета производный
void Dummy_flow::calculate_dir() {
    //для всех частиц - внешний цикл
    for (int i = 0; i < N; ++i) {

        double p_dir = 0;
        double v_dir = 0;
        double e_dir = 0;

        //расчет взаимодействия i частицы со всеми остальными
        for (int j = 0; j < N; ++j) {
            //каждая производная определяется суммой i частицы со всеми остальными
            p_dir += two_part_p(data[i], data[j]);
            v_dir += two_part_v(data[i], data[j]);
            e_dir += two_part_energy(data[i], data[j]);
        }

        data[i].set_p_dir(p_dir);
        data[i].set_vx_dir(v_dir);
        data[i].set_e_dir(e_dir);
    }
}

void Dummy_flow::calculate_final() {
    double cur_maxV = 0;
    double cur_maxC = 0;
    double cur_maxP = 0;
    double cur_minP = 100500;

    for (int i = 0; i < N; ++i) {
        data[i].ronge_kutt(dt);

        cur_maxV = data[i].V();
        cur_maxC = data[i].C();
        cur_maxP = data[i].P();
        cur_minP = data[i].P();
        if (cur_maxV > max_V) max_V = cur_maxV;
        if (cur_maxC > max_C) max_C = cur_maxC;
        if (cur_maxP > max_P) max_P = cur_maxP;
        if (cur_minP < min_P) min_P = cur_minP;

        if (abs(data[i].X() - left) < h || abs(data[i].X() - right) < h) {
            data[i].set_vx(-data[i].Vx());
        }
    }
    iteration++;
    curent_time += dt;
}


//Рассчитываются производные, а затем финальные значения
void Dummy_flow::calculate() {
    calculate_dir();
    calculate_final();
    curent_time += dt;
}