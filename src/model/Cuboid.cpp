/*
 * Cuboid.cpp
 *
 *  Created on: 15.11.2022
 *      Author: borisov
 */

#include "./Cuboid.h"

Cuboid::Cuboid(std::array<double, 3> x, std::array<int, 3> n, double h, double m, std::array<double, 3> v, double meanV) {
    this->x = x;
    this->n = n;
    this->h = h;
    this->m = m;
    this->v = v;
    this->meanV = meanV;
    _memoryLogger = spdlog::get("memory_logger"); 
    _memoryLogger->info("Cuboid generated!"); 
}


Cuboid::~Cuboid(){
    _memoryLogger->info("Cuboid destructed!"); 
}

/*
* Getters
*/

const std::array<double, 3> Cuboid::getX() {
    return this->x;
}

const std::array<int, 3> Cuboid::getN() {
    return this->n;
}

const double Cuboid::getH() {
    return this->h;
}

const double Cuboid::getM() {
    return this->m;
}

const std::array<double, 3> Cuboid::getV() {
    return this->v;
}

const double Cuboid::getMeanV() {
    return this->meanV;
}

/*
* Setters
*/

const void Cuboid::setX(std::array<double, 3> &x) {
    this->x = x;
}

const void Cuboid::setN(std::array<int, 3> &n) {
    this->n = n;
}

const void Cuboid::setH(double h) {
    this->h = h;
}

const void Cuboid::setM(double m) {
    this->m = m;
}

const void Cuboid::setV(std::array<double, 3> &v) {
    this->v = v;
}

const void Cuboid::setMeanV(double meanV) {
    this->meanV = meanV;
}
