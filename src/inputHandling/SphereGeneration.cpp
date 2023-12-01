#include "SphereGeneration.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "cmath"

void addSpheres(SimulationContainer& particleContainer, std::list<FileReader::SphereData> spheres) {

    //determine total amount of particles that will be generated
    //not accurate, the reserved amount will be slightly larger than the amount of actual particles needed

    size_t dim{3};
    size_t needed_capacity{0};
    double z =spheres.front().CenterPosition[1];
    for (auto &sphere : spheres) {
        needed_capacity += (int)(4/3* pow(sphere.radius,3)/pow(sphere.meshWidth,3));
    }

    //allocate the needed amount of memory in the beginning
    //to avoid copying with shrink_to_fit
    //particleContainer.reserve(needed_capacity);

    for (auto &sphere : spheres) {
        generateSpheresMethod2(sphere, particleContainer);
    }
}

void generateSpheresMethod1(FileReader::SphereData& sphere, SimulationContainer& particleContainer){

    // dont need Maxwell Boltzmann Distribution in this Sphere assignment but can be added later if needed
    // std::array<double, 3> dist(maxwellBoltzmannDistributedVelocity(sphere.avg_v, dim));

    for (double z = 0; z <= sphere.radius;) {

        for (double y=0; y <= sqrt(pow(sphere.radius, 2)-pow(z, 2));) {

            for (double x = 0; x  <= sqrt(pow(sphere.radius, 2)-pow(z, 2)-pow(y,2));) {
                std::array<double, 3> cords(sphere.CenterPosition);
                std::array<double, 3> vel(sphere.Velocity);
                cords[0]+=x;
                cords[1]+=y;
                cords[2]+=z;
                particleContainer.addParticle(cords, vel, sphere.mass); //(x,y,z)
                cords[0]-=2*x;
                particleContainer.addParticle(cords, vel, sphere.mass); //(-x,y,z)
                cords[1]-=2*y;
                particleContainer.addParticle(cords, vel, sphere.mass);//(-x,-y,z)
                cords[2]-=2*z;
                particleContainer.addParticle(cords, vel, sphere.mass);//(-x,-y,-z)
                cords[0]+=2*x;
                particleContainer.addParticle(cords, vel, sphere.mass); //(x,-y,-z)
                cords[1]+=2*y;
                particleContainer.addParticle(cords, vel, sphere.mass); //(x,y,-z)
                cords[1]-=2*y;
                cords[2]+=2*z;
                particleContainer.addParticle(cords, vel, sphere.mass); //(x,-y,z)
                cords[0]-=2*x;
                cords[1]+=2*y;
                cords[2]-=2*z;
                particleContainer.addParticle(cords, vel, sphere.mass); //(-x,y,-z)
                x=x+sphere.meshWidth;
            }
            y=y+sphere.meshWidth;
        }
        z=z+sphere.meshWidth;
    }
}

void generateSpheresMethod2(FileReader::SphereData& sphere, SimulationContainer& particleContainer){
    uint64_t neededN = (uint64_t) (sphere.radius/sphere.meshWidth);
    std::array<double, 3> startingPoint;
    startingPoint[0]=sphere.CenterPosition[0]-sphere.radius;
    startingPoint[1]=sphere.CenterPosition[1]-sphere.radius;
    startingPoint[2]=sphere.CenterPosition[2]-sphere.radius;
    for(uint64_t z = 0; z < neededN; z++) {

        for (uint64_t y = 0; y < neededN; y++) {

            for (uint64_t x = 0; x < neededN; x++) {
                std::array<double, 3> cords(startingPoint);
                std::array<double, 3> vel(sphere.Velocity);

                cords[0] += x * sphere.meshWidth;
                cords[1] += y * sphere.meshWidth;
                cords[2] += z * sphere.meshWidth;

                if (pow(cords[0]-sphere.CenterPosition[0],2)+pow(cords[1]-sphere.CenterPosition[1],2)
                +pow(cords[2]-sphere.CenterPosition[2],2)< pow(sphere.radius,2))
                particleContainer.addParticle(cords, vel, sphere.meshWidth);

            }
        }
    }
}