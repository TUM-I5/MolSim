#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/inputReader/XMLInputReader.h"

// correctness of xml parsing
TEST(XMLInputReader, XMLParsing)
{
    std::shared_ptr<ProgramParameters> pp = std::make_shared<ProgramParameters>();
    std::unique_ptr<XMLInputReader> xml = std::make_unique<XMLInputReader>();

    const char *file = "../tests/Simulation.xml";
    xml->readInput(*pp, file);

    EXPECT_THAT(pp->getEndTime(), 10);
    EXPECT_THAT(pp->getDeltaT(), 0.00005);
    EXPECT_THAT(pp->getSigma(), 1);
    EXPECT_THAT(pp->getEpsilon(), 5);
    EXPECT_THAT(pp->getCutoff(), 3.0);
    BoundaryCondition b = pp->getBoundaries()[2];
    EXPECT_THAT(b, BoundaryCondition::Reflecting);
    EXPECT_THAT(pp->getTempInit(), 0);
    EXPECT_THAT(pp->getBrownianMotion(), false);
    EXPECT_THAT(pp->getNThermostats(), 0);
    EXPECT_THAT(pp->getTempTarget(), 0);
    EXPECT_THAT(pp->getGGrav(), 0);
}