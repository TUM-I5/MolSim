//
// Created by Alp Kaan Aksu on 01.11.23.
//

#ifndef PSEMOLDYN_GROUPF_MODEL_H
#define PSEMOLDYN_GROUPF_MODEL_H

#include <utility>

#include "functional"
#include "Particle.h"
#include "utils/ArrayUtils.h"

class Model {
private:
    std::function<void(Particle&, Particle&)> force;
    std::function<void(Particle&)> position;
    std::function<void(Particle&)> velocity;

public:
    Model(std::function<void(Particle&, Particle&)> force, std::function<void(Particle&)> position, std::function<void(Particle&)> velocity) : force(std::move(force)), position(std::move(position)), velocity(std::move(velocity)) {};

    static std::function<void(Particle&)> resetForceFunction() {
        return [](Particle &p) {
            p.updateF(std::array<double, 3> {0., 0., 0.});
        };
    };

    std::function<void(Particle&, Particle&)> forceFunction() {
        return force;
    }

    std::function<void(Particle&)> positionFunction() {
        return position;
    }

    std::function<void(Particle&)> velocityFunction() {
        return velocity;
    }

    static Model basicModel(double deltaT) {
        auto force = [](Particle &p1, Particle &p2) {
            auto nextForce =
                    p1.getM() * p2.getM() / std::pow(p2.distanceTo(p1), 3) * p1.diffTo(p2);

            p1.setF(p1.getF() + nextForce);
        };

        auto position = [deltaT](Particle &p) {
            auto x =
                    p.getX() +
                    deltaT * p.getV() +
                    (deltaT * deltaT / (2 * p.getM())) * p.getF();

            p.setX(x);
        };

        auto velocity = [deltaT](Particle &p) {
            auto v =
                    p.getV() +
                    (deltaT / (2 * p.getM())) * (p.getOldF() + p.getF());

            p.setV(v);
        };

        return Model{force, position, velocity};
    };
};

#endif //PSEMOLDYN_GROUPF_MODEL_H
