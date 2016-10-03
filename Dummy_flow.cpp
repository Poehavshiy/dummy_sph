//
// Created by nikita on 29.09.16.
//

#include "Dummy_flow.h"

// производная сглаживающей функции (формула 3.81) со страницы 92
double new_quartic(double R, double h) {
    if (R > 2.0) return 0.0;
    double a = -2.0 / (3.0 * h * h);
    double result = 0;

    if (R <= 0.66666) result = 1;
    else if (R > 0.66666 && R <= 1.0) result = 3.0 * R * (4.0 - 3.0 * R) / 4.0;
    else if (R > 1.0 && R < 2.0) result = 3.0 * pow((2.0 - R), 2.0) / 4.0;

    return result * a;
}
//
double Dummy_flow::grad_w_test(double x) {
    double minR = 0.00001;
    //if (fabs(x) < minR) return 0;
    double r = fabs(x) / h;
    double sign = (x > 0) ? 1 : ((x < 0) ? -1 : 0);//fsign(x);// / fabs(x);
    double result = new_quartic(r, h);
    return result * sign;
}

double Dummy_flow::two_part_art_visc(const Particle &a, const Particle &b) {
    double scalar = (a.Vx() - b.Vx()) * (a.X() - b.X());
    if (scalar >= 0) return 0;
    double gama = 1.4;
    double alpha = 1;
    double beta = 2;
    double e2 = 0.01;
    double uij = h * scalar / ( pow(a.X() - b.X(), 2) + e2 + h * h );

    //double aPp = fabs( a.P() / a.p() );
    //double bPp = fabs( b.P() / b.p() );
    double aPp = a.P() / a.p();
    double bPp = b.P() / b.p();
    /*if(aPp<0)
        aPp = 0;
    if(bPp<0)
        bPp = 0;
*/
    double brackets = pow(gama * aPp, 0.5) + pow(gama * bPp, 0.5);

    double result = 2 * ( -uij * alpha * 0.5 * brackets + beta * pow(uij, 2) ) / ( a.p() + b.p() );
    return result;
}
/*
double Dummy_flow::two_part_art_visc(const Particle &a, const Particle &b) {
    double delta_vx;
    double deltaX;
    deltaX = a.X() - b.X();
    delta_vx = a.Vx() - b.Vx();
    double artvis_alpha = 1;
    double artvis_beta = 1;
    double artvis_phi = 0.1*h;
    double scalar_product = delta_vx * deltaX;
    if (scalar_product > 0) return 0;
    double r = deltaX;
    double phi = h * scalar_product / ( pow(r, 2) + pow(artvis_phi, 2) );
    double c_ab = 0.5 * (a.C() + b.C());
    double p_ab = 0.5 * (a.p() + b.p());
    double result = (-artvis_alpha * c_ab * phi + artvis_beta * pow(phi, 2)) / p_ab;

    return result;
}
*/

Dummy_flow::Dummy_flow() {
    /*
      iteration = 0;

      curent_time = 0;

      dt = 0.005;

      left = -0.6;
      right = 0.6;

      N = 400;

      h = (abs(left - right) / N) * 1;
      h = 0.5/80 * 3;
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
      }*/

    iteration = 0;

    curent_time = 0;

    dt = 0.0001;

    left = 0;
    right = 1;

    N = 130;
    h = 0.5 / 80.0 * 3;

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
        double cur_x = last_x + step2 * i;
        data[bigPN + ghoustN + i].set_pos(cur_x, 0);
    }

}
//производная W собственно сама функция вычисляется в new_quartic
//здесь этот результат умножается на (xi-xj)/r -пременная фактор
//это соответствует способу вычисления производной W по формуле 2.23 со страницы 43

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
    double brackets1 = a.P() / pow(a.p(), 2.0) + b.P() / pow(b.p(), 2.0) + two_part_art_visc(a, b);
    //
    double res = Mj * brackets1 * Wij * -1;//
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
    double brackets1 = a.P() / pow(a.p(), 2.0) + b.P() / pow(b.p(), 2.0) + two_part_art_visc(a, b);
    //
    double res = 0.5 * VijWij * Mj * brackets1;

    return res;
}


//функция расчета производный
void Dummy_flow::calculate_dir() {
    //для всех частиц - внешний цикл
    //for (int i = ghoustN; i < N - ghoustN; ++i) {
    for (int i = ghoustN; i < N - ghoustN; ++i) {
        double p_dir = 0;
        double v_dir = 0;
        double e_dir = 0;

        //расчет взаимодействия i частицы со всеми остальными
        for (int j = 0; j < N; ++j) {
            //каждая производная определяется суммой i частицы со всеми остальными
            if (i != j) {
                p_dir += two_part_p(data[j],data[i]);
                v_dir += two_part_v(data[i], data[j]);
                e_dir += two_part_energy(data[i], data[j]);
                /*
                if(two_part_energy(data[i], data[j]) -two_part_energy(data[j], data[i]) > 0.000000001||
                        two_part_v(data[i], data[j]) -two_part_v(data[j], data[i]) > 0.000000001||
                        two_part_p(data[i], data[j]) -two_part_p(data[j], data[i]) > 0.000000001){

                    double eij = two_part_energy(data[i], data[j]);
                    double eji = two_part_energy(data[j], data[i]);

                    double vij = two_part_v(data[i], data[j]);
                    double vji = two_part_v(data[j], data[i]);

                    double pij = two_part_p(data[i], data[j]);
                    double pji = two_part_p(data[j], data[i]);

                    int fff = 666;
                }
                 */
            }
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

    for (int i = ghoustN; i < N - ghoustN; ++i) {

        data[i].ronge_kutt(dt);

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
    iteration++;
    curent_time += dt;
}