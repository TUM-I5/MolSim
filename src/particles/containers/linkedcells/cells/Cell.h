#pragma once

#include <array>
#include <optional>
#include <unordered_set>
#include <vector>

#include "particles/Particle.h"

class Cell {
   public:
    enum class CellType { INNER, BOUNDARY, HALO };
    enum class BoundaryType { OUTFLOW, REFLECTING };

   private:
    CellType cell_type;
    std::vector<Particle*> particle_references;
    std::vector<Cell*> neighbour_references;
    std::unordered_set<Cell*> already_influenced_by;

   public:
    Cell(CellType cell_type);

    CellType getCellType() const;
    std::vector<Particle*>& getParticleReferences();
    std::vector<Cell*>& getNeighbourReferences();
    std::unordered_set<Cell*>& getAlreadyInfluencedBy();

    void addParticleReference(Particle* p);
    void clearParticleReferences();

    void addNeighbourReference(Cell* c);

    void addAlreadyInfluencedBy(Cell* c);
    void clearAlreadyInfluencedBy();
};