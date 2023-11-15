
#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() : particles() {}

ParticleContainer::~ParticleContainer() {}
/**
 * @brief provides all possible particle pairs based on the handshake problem
 *
 * particle pairs are based on a "triangle" on the matrix of all
 * possible pair combinations, to avoid redundant calls leveraging
 * F_ij = -F_ji. Sets next pair of particles to calculate the force
 * in between of them or to a pair of nullptr to indicate the end
 * and later for synchronisation of threads.
 *
 * @param std::pair<Particle*, Particle*> *pair to set the next
 * pair of pointers into
 */
void ParticleContainer::setNextPair(std::pair<Particle *, Particle *> &pair) {
  static size_t row = 0;
  static size_t column = 1;

  if (column < particles.size()) {
    pair.first = &particles[row];
    pair.second = &particles[column];
    column++;

  } else {
    row++;
    if (row < particles.size() - 1) {
      column = row + 1;
      pair.first = &particles[row];
      pair.second = &particles[column];
      column++;

    } else {
      row = 0;
      column = 1;
      pair.first = nullptr;
      pair.second = nullptr;
    }
  }
}

/**
 * @brief provides the next particle iteration
 *
 * returns nullptr to indicate the end of the particle list. It will
 * be used for synchronisation of threads later on.
 *
 * @return Particle pointer or nullptr
 */
Particle *ParticleContainer::getNextParticle() {
  static size_t count = 0;

  if (count < particles.size()) {
    return &particles[count++];
  }

  count = 0;
  return nullptr;
}

/**
 * @brief creates and adds particle with provided parameters
 *
 * @param x_arg position of the particle
 * @param v_arg velocity of the particle
 * @param m_arg mass of the particle
 */
void ParticleContainer::addParticle(std::array<double, 3> x_arg,
                                    std::array<double, 3> v_arg, double m_arg) {
  particles.emplace_back(x_arg, v_arg, m_arg);
}

/**
 * @brief reserves space in the currently used vector
 *
 * used to avoid unallocated reserved space and copying of particles
 * through the vectors dynamic size adaptation
 *
 * @param capacity amount of space to reserve
 */
void ParticleContainer::reserve(size_t capacity) {
  particles.reserve(capacity);
}

Particle &ParticleContainer::operator[](size_t i) { return particles[i]; }

/**
 * @brief plots all particles into a VTK output
 *
 * @param writer to provide plotting method
 */
void ParticleContainer::plotParticles(outputWriter::VTKWriter &writer) {
  for (auto &particle : particles) {
    writer.plotParticle(particle);
  }
}

std::string ParticleContainer::to_string() {
  std::ostringstream particles_string;
  for (auto &particle : particles) {
    particles_string << particle << '\n';
    //std::cout << "Adding particle" << '\n';
  }

  return particles_string.str();
}

size_t ParticleContainer::size() const { return particles.size(); };
