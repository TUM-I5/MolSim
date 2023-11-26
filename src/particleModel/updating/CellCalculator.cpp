#include "CellCalculator.h"

//todo check if particles not copied
CellCalculator::CellCalculator(CellContainer &cellContainer, const double delta_t)
    : cellContainer(cellContainer), delta_t(delta_t), particles(*cellContainer.getParticles()){}

void CellCalculator::initializeF() {
    static std::array<dim_t, 3> pos;
    cellContainer.setNextCell(pos);
    //todo
}

void CellCalculator::calculateLinkedCellF() {
    //todo
}

void CellCalculator::calculateWithinVXF() {
    //todo include shiftF in the beginning
    //todo
}
