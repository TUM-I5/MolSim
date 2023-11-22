#include "Cell.h"

Cell::Cell(CellType cell_type, std::optional<BoundaryType> boundary_type) : cell_type(cell_type), boundary_type(boundary_type) {}

Cell::CellType Cell::getCellType() const { return cell_type; }

std::vector<Particle*>& Cell::getParticleReferences() { return particle_references; }

void Cell::addParticleReference(Particle* p) { particle_references.push_back(p); }

void Cell::clearParticleReferences() { particle_references.clear(); }