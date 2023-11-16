//
// Created by Layla Zadina on 09.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_PARTICLEGENERATOR_H
#define PSEMOLDYN_GROUPE_PARTICLEGENERATOR_H

#include <array>


class ParticleGenerator {

private:
    /** Number of particles per dimension: N1 × N2 × N3
    // Distance h of the particles (mesh width of the grid)
    // Mass m of one particle
    // Initial velocity v of the particles (3 Components)
     The coordinate of the lower left front-side corner (x,y,z)
     */

    int N1;
    int N2;
    int N3;
    double h;
    double m;
    std::array<double, 3> &v;
    double x;
    double y;
    double z;
    int type;

public:
    /**
     * create a N1xN2xN3 grid of particles with mass m, initial velocity v, left corner coordinate (x,y,z)
    */

    ParticleGenerator(int n1, int n2, int n3, double h1, double m1, std::array<double, 3> v1,
                      double x1, double y1, double z1, int type1);


};


#endif //PSEMOLDYN_GROUPE_PARTICLEGENERATOR_H
