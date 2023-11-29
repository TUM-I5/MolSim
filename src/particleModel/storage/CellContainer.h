#pragma once

#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include "SimulationContainer.h"
#include <vector>

typedef int dim_t;
extern dim_t dim_t_res;

class CellContainer : public SimulationContainer {
public:
    CellContainer(double domain_width, double domain_height, double domain_depth, double r_cutoff, double cell_size);

    virtual ~CellContainer();



    class BoundaryIterator{
        public:
            dim_t x = 1;
            dim_t y = 1;
            dim_t z = 1;
            CellContainer& cell;

        BoundaryIterator(CellContainer& cell_cont) : cell(cell_cont) {};
        BoundaryIterator(CellContainer& cell_cont,dim_t x_,dim_t y_, dim_t z_) :  x(x_) , y(y_) , z(z_) , cell(cell_cont)  {};

        BoundaryIterator& operator++();

        std::vector<Particle>& operator*();

        bool operator==(const BoundaryIterator& other);
        bool operator!=(const BoundaryIterator& other);
    };

    BoundaryIterator begin_boundary();

    BoundaryIterator end_boundary();

    void setNextCell(std::array<dim_t, 3> &next_position);

    void setNextPath(std::array<dim_t, 3> &start, std::array<dim_t, 3> &pattern);

    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg) override;

    void plotParticles(outputWriter::VTKWriter &writer) override;

    void updateParticleCell(Particle& particle,std::array<dim_t, 3>& cell);

    void iterateNextBoundaryCell3D(std::array<dim_t, 3> &next_position);

    void next_correct_boundary_index(dim_t& x,dim_t& y,dim_t& z);

    std::string to_string() override;

    void reserve(size_t) override;     

    size_t size() override;

    auto getParticles() {
        return &particles;
    }

protected:
    bool three_dimensions;
    double cell_size;
    std::array<double, 3> domain_borders;
    dim_t comparing_depth = 1;
    size_t particle_amount = 0;
    std::vector<std::vector<std::vector<std::vector<Particle>>>> particles;

    void setNext3dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    void setNext2dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start);

    void allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position);
};

