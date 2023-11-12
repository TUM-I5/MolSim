#include "Simulation.h"

namespace Simulation {

std::shared_ptr<spdlog::logger> simulation_log =
    spdlog::basic_logger_mt("Simulation_log1", "logging/simulation-log.txt");

void runSimulation(ParticleContainer &particleContainer, const double end_time,
                   const double delta_t) {
  outputWriter::VTKWriter writer;
  Model model(particleContainer, "LennJones", delta_t);

  double current_time = 0;
  int iteration = 0;

  // calculate inital force:
  model.calculateF();
  model.shiftForces();
  //std::cout << particleContainer.to_string() << '\n';
  simulation_log->debug("Particles that are simulated:\n");
  simulation_log->debug(particleContainer.to_string());


  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    model.calculateX();

    model.calculateF();

    model.calculateV();

    iteration++;

    if (iteration % 10 == 0) {
      writer.initializeOutput(particleContainer.size());
      particleContainer.plotParticles(writer);
      writer.writeFile("out", iteration);
    }

    model.shiftForces();

    /// loading bar
    if (iteration % 50 == 0) {
      size_t barWidth = 50;
      size_t pos = static_cast<size_t>(barWidth * (current_time / end_time));

      std::cout << "[" << std::string(pos, '=') << '>'
                << std::string(barWidth - pos, ' ') << "] "
                << int((current_time / end_time) * 100.0) << "%\r"
                << std::flush;
    }

    current_time += delta_t;
  }

  std::cout << "[" << std::string(50, '=') << ">] " << 100 << "%\r"
            << std::flush;
  std::cout << "\noutput written. Terminating...\r" << std::endl;
}

void plotParticles(ParticleContainer &particleContainer, int iteration) {
  std::string out_name("MD_vtk");
  outputWriter::XYZWriter writer;
  writer.plotParticles(particleContainer, out_name, iteration);
}
}  // namespace Simulation