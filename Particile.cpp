//
// Created by nikita on 13.05.16.
//

#include "Particile.h"

Particle::Particle() : pos(0, 0) {

    isBoundary = 0;
    //
    density = 0;
    pressure = 0;
    energy = 0;
    //
    vx = 0;
    vy = 0;
    //
    mass = 0;

};

Particle::Particle(bool status, double p,
                   double P, double E, double Vx, double Vy, double M) {

    isBoundary = status;
    density = p;
    pressure = P;
    energy = E;
    //
    vx = Vx;
    vy = Vy;
    //
    mass = M;
    //
    density_dir = 0;
    energy_dir = 0;
    vx_dir = 0;
    vy_dir = 0;
};

//пересчет значений
void Particle::ronge_kutt(double dt) {
    pressure = energy * (k_costil - 1) * density;
    pos.x += dt * vx;
    //

    density += dt * density_dir;
    if (density < 0) {
        Particle a = *this;
        throw a;
    }

    energy += dt * energy_dir;
    vy = 0;
    vx += dt * vx_dir;
    pos.y = 0;

    //обнулим производные
    density_dir = 0;
    vx_dir = 0;
    energy_dir = 0;
}

double Particle::delta_V(const Particle &right) {
    return this->vx - right.vx;
}

double Particle::delta_x(const Particle &right) {
    return this->pos.x - right.pos.x;
}

double Particle::sign(const Particle &right) {
    double x = delta_x(right);
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

double Particle::dW(const Particle &right) {
    double r = this->pos.x - right.pos.x;
    return dWf.dW_Bykov(r, h);
}

double Particle::art_viscous_Bykov(const Particle &part_j) {
    double scalar = delta_V(part_j) * delta_x(part_j);
    if (delta_V(part_j) * sign(part_j) < 0) {
        double gamma = 1.4;
        double alpha = 1.0;
        double betta = 2.0;
        double e2  = 0.01;
        double mu_ij = (h * scalar) / (delta_x(part_j) * delta_x(part_j) + h * h * e2);
        double res = 2 * ((-mu_ij * alpha * 0.5 *
                           (sqrt(gamma * pressure / density) + sqrt(gamma * part_j.pressure / part_j.density)) +
                           betta * mu_ij * mu_ij) / (density + part_j.density));
        return res;
    }
    return 0;
}

double Particle::art_viscous__Monaghan(const Particle& part_j){
    double divv = delta_V(part_j);
    double divx = delta_x(part_j);
    double scalar = divv*divx;
    if( scalar >= 0) return 0;

    double alpha = 1.0;
    double betta = 1.0;

    double phi = h * scalar / (pow(divx, 2) + pow(0.1 * h, 2));
    double c = 0.5 * (C() + part_j.C());
    double p = 0.5 * (density + part_j.density);
    double result;
    result = (-alpha * c * phi + betta * phi * phi)/p;
    return result;
}

void Particle::derivatives_from_particle(const Particle &part_j,const int& i, const int& j, const int& iteration) {
    double dW_ = dW(part_j);
    if(fabs(dW_) < 0.00001) return;

    if(i == 95 && iteration > 0){
        int check = 1;
    }
    double deltaV_ = delta_V(part_j);
    double sign_ = sign(part_j);
    double Mj = part_j.mass;
    double viscous = art_viscous_Bykov(part_j);
    viscous = art_viscous__Monaghan(part_j);
    //
    double brackets1 = Mj * sign_ * dW_;
    double brackets2 = part_j.pressure / (part_j.density * part_j.density) +
                       pressure / (density * density) +
                       viscous;

    double dp = brackets1 * deltaV_;
    double dV = -brackets1 * brackets2;
    double de = brackets1 * brackets2 * deltaV_ * 0.5;
    //
    density_dir += dp;
    vx_dir += dV;
    energy_dir += de;
}