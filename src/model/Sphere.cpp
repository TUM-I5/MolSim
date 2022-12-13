/*
 * Sphere.cpp
 *
 *  Created on: 02.02.2022
 *      Author: wohlrapp
 */

#include "./Sphere.h"

Sphere::Sphere(std::array<double, 3> center, int r, double h, double m, std::array<double, 3> v, double meanV, double epsilon, double sigma, int type)
{
    this->center = center;
    this->r = r;
    this->h = h;
    this->m = m;
    this->v = v;
    this->meanV = meanV;
    this->epsilon = epsilon;
    this->sigma = sigma;
    this->type = type;
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("Sphere generated!");
}

Sphere::~Sphere()
{
    _memoryLogger->info("Sphere destructed!");
}

/*
 * Getters
 */

const std::array<double, 3> Sphere::getCenter()
{
    return this->center;
}

const int Sphere::getR()
{
    return this->r;
}

const double Sphere::getH()
{
    return this->h;
}

const double Sphere::getM()
{
    return this->m;
}

const std::array<double, 3> Sphere::getV()
{
    return this->v;
}

const double Sphere::getMeanV()
{
    return this->meanV;
}

const double Sphere::getEpsilon() const
{
    return this->epsilon;
}

const double Sphere::getSigma() const
{
    return this->sigma;
}

const int Sphere::getType()
{
    return this->type;
}

/*
 * Setters
 */

const void Sphere::setCenter(std::array<double, 3> &center)
{
    this->center = center;
}

const void Sphere::setR(int r)
{
    this->r = r;
}

const void Sphere::setH(double h)
{
    this->h = h;
}

const void Sphere::setM(double m)
{
    this->m = m;
}

const void Sphere::setV(std::array<double, 3> &v)
{
    this->v = v;
}

const void Sphere::setMeanV(double meanV)
{
    this->meanV = meanV;
}

const void Sphere::setType(int type)
{
    this->type = type;
}
