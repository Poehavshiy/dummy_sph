//
// Created by nikita on 13.05.16.
//

#ifndef SPHSM6_PARTICILE_H
#define SPHSM6_PARTICILE_H

//
#include "dW_functions.h"
//
//QTшные либы
using namespace std;

enum DERIVATIVES {
    DENSITY, VX, VY, ENERGY
};
//

struct Point {
    Point() {
        x = 0;
        y = 0;
    }

    Point(double X, double Y) {
        x = X;
        y = Y;
    }

    double x;
    double y;

    friend std::istream &operator<<(ostream &is, Point &income) {
        is << income.x << ' ' << income.y;
    }

    friend bool operator==(const Point &left, const Point &right) {
        if ((left.x == right.x) && (left.y == right.y)) return true;
        else return false;
    }
};

class Particle {
    static dW_functions dWf;
    bool isBoundary;
    //1 if its boundary cell
    //data
    double mass;
    //position
    Point pos;
    double molar_mass = 0.029;
    //
    double density;
    double pressure;
    double energy;
    //
    double vx;
    double vy;
    //
    //производные
    double density_dir;
    double energy_dir;
    double vx_dir;
    double vy_dir;

    static constexpr double R_costil = 8.31;

    static constexpr double k_costil = 1.4;

    static constexpr double molar = 0.029;

    double delta_V(const Particle& right);

    double delta_x(const Particle& right);

    double sign(const Particle& right);

    double dW(const Particle& right);

    double art_viscous_Bykov(const Particle& right);

    double art_viscous__Monaghan(const Particle& right);

    double h = 0.5 / 80.0 * 3;


public:
    Particle();

    Particle(bool status, double p = 0,
             double P = 0, double E = 0, double Vx = 0, double Vy = 0, double M = 0);

    //get P, Vx, Vy, E, p
    double P() const {
        return pressure;
    }

    double Vx() const{
        return vx;
    }

    double Vy() const{
        return vy;
    }

    double p() const{
        return density;
    }

    double E() const{
        return energy;
    }

    double V(){
        return hypot(vx, vy);
    }

    double X() const {
        return pos.x;
    }

    double Y() const {
        return pos.y;
    }

    double T() const {
        return pressure * molar_mass / (density * R_costil);
    }

    double C() const {
        return sqrt(k_costil * (k_costil - 1) * energy);
    }

    void set_pos(double X, double Y) {
        pos.x = X;
        pos.y = Y;
    }
    //пересчет значений
    void ronge_kutt(double dt);

    bool boundary_status() {
        return isBoundary;
    }
    // рассчетные формулы
    void derivatives_from_particle(const Particle& right,const int& i = 0, const int& j = 0, const int& iteration=0);
};


#endif //SPHSM6_PARTICILE_H
