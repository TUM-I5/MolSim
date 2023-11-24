#pragma once

#include <array>
#include <optional>
#include <vector>

#include "particles/Particle.h"

class Cell {
   public:
    enum class CellType { INNER, BOUNDARY, HALO };
    enum class BoundaryType { OUTFLOW, REFLECTING };

   private:
    CellType cell_type;
    std::optional<BoundaryType> boundary_type;
    std::vector<Particle*> particle_references;
    std::vector<Cell*> neighbour_references;

    std::vector<Cell*> already_influenced_by;

   public:
    Cell(CellType cell_type, std::optional<BoundaryType> boundary_type = std::nullopt);

    CellType getCellType() const;
    std::vector<Particle*>& getParticleReferences();
    std::vector<Cell*>& getNeighbourReferences();
    std::vector<Cell*>& getAlreadyInfluencedBy();

    void addParticleReference(Particle* p);
    void clearParticleReferences();

    void addNeighbourReference(Cell* c);

    void addAlreadyInfluencedBy(Cell* c);
    void clearAlreadyInfluencedBy();
};