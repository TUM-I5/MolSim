
#pragma once

#include <list>
#include <sstream>
#include <string>

#include "particleModel/storage/Particle.h"
#include "particleModel/storage/ParticleContainer.h"
#include "particleModel/updating/CellCalculator.h"


class FileReader {
 public:
  FileReader();
  virtual ~FileReader();

  struct SphereData {
    std::array<double, 3> CenterPosition;
    std::array<double, 3> Velocity;
    double mass;
    double radius;
    double meshWidth;
    double sigma;
    double epsilon;

    std::string to_string() const{
     auto sphereData = (*this);
    std::ostringstream oss;

    oss << "  SphereData:" << std::endl;
    oss << "  center: (" << sphereData.CenterPosition[0] << ", "
        << sphereData.CenterPosition[1] << ", " << sphereData.CenterPosition[2] << ")" << std::endl;
    oss << "  velocity: (" << sphereData.Velocity[0] << ", "
        << sphereData.Velocity[1] << ", " << sphereData.Velocity[2] << ")" << std::endl;
    oss << "  mass: " << sphereData.mass << std::endl;
    oss << "  radius: " << sphereData.radius << std::endl;
    oss << "  mesh width: " << sphereData.meshWidth << std::endl;
    oss << "  sigma: " << sphereData.sigma << std::endl;
    oss << "  epsilon: " << sphereData.epsilon << std::endl;

    return oss.str();
    }

    bool operator==(const SphereData& other) const {
        return (CenterPosition == other.CenterPosition &&
                Velocity == other.Velocity &&
                mass == other.mass &&
                radius == other.radius &&
                meshWidth == other.meshWidth &&
                sigma == other.sigma &&
                epsilon == other.epsilon);
    }


  };

  struct CuboidData {
    /// initial velocity and position vectors
    std::array<double, 3> x, v;

    /// N1: amount of particles along dimension 1
    /// N2: amount of particles along dimension 2
    /// N3: amount of particles along dimension 3
    uint64_t N1, N2, N3;

    /// Mass m of the particles in the cuboid
    /// Mesh width h
    double m, h;

    /// sigma and epsilon parameters for the force calculation
    /// between particles of this cuboid
    double sigma, epsilon;

    /// Average velocity (always 0.1 in our case)
    double avg_v = 0.1;

    /**
     * @brief Convert CuboidData to a string
     *
     * @return String representation of CuboidData
     */
    std::string to_string() const {
      std::stringstream ss;

      ss << "CuboidData:" << std::endl;
      ss << "  x: (" << x[0] << ", " << x[1] << ", " << x[2] << ")"
         << std::endl;
      ss << "  v: (" << v[0] << ", " << v[1] << ", " << v[2] << ")"
         << std::endl;
      ss << "  N1: " << N1 << std::endl;
      ss << "  N2: " << N2 << std::endl;
      ss << "  N3: " << N3 << std::endl;
      ss << "  m: " << m << std::endl;
      ss << "  h: " << h << std::endl;
      ss << "  sigma: " << sigma << std::endl;
      ss << "  epsilon: " << epsilon << std::endl;

      return ss.str();
    }

    /**
     * @brief Compare two CuboidData structs. Used for Testing
     *
     * @return True if this CuboidData struct has the same content as other
     */
    bool operator==(const CuboidData &other) const {
      return (x == other.x && v == other.v && N1 == other.N1 &&
              N2 == other.N2 && N3 == other.N3 && m == other.m &&
              h == other.h && sigma == other.sigma &&
              epsilon == other.epsilon && avg_v == other.avg_v);
    }
  };



  struct ProgramArgs {
    // arguments of the simulation
    double delta_t;
    double t_end;
    double cut_of_radius;
    double cell_size;
    std::array<boundary_conditions,6> boundaries;
    std::array<double,3> domain_dimensions;

    std::string file_basename = "out";
    size_t write_frequency = 10;

    // spheres and cuboids to simulate
    std::list<CuboidData> cuboids;
    std::list<SphereData> spheres;
  

    std::string to_string() {
    
    std::ostringstream oss;

    oss << "Delta_t: " << delta_t << std::endl;
    oss << "T_end: " << t_end << std::endl;
    oss << "File basename: " << file_basename << std::endl;
    oss << "Write frequency: " << write_frequency << std::endl;

    oss << "Boundary conditions: [" << std::endl;
    int side = 0;
    for (const auto condition : boundaries) {
        std::string condition_name;
        std::string side_name;
        switch(condition) {
            case boundary_conditions::reflective:
                condition_name = "reflective";
                break;
            case boundary_conditions::outflow:
                condition_name = "outflow";
                break;
            default:
                condition_name = "undefined";

        }
        switch(side) {
            case(0):
                side_name = "Positive Z: ";
                break;
            case(1):
                side_name = "Negative Z: ";
                break;
            case(2):
                side_name = "Positive X: ";
                break;
            case(3):
                side_name = "Negative X: ";
                break;
            case(4):
                side_name = "Positive Y: ";
                break;
            case(5):
                side_name = "Negative Y: ";
                break;
        }

        oss << side_name << condition_name << std::endl;
        side++;
    }
    oss << "]" << std::endl;

    oss << "Spheres: [" << std::endl;
    for (const auto& sphere : spheres) {
        oss <<  sphere.to_string() << std::endl;
    }
    oss << "]" << std::endl;

    oss << "Cuboids: [" << std::endl;
    for (const auto& cuboid : cuboids) {
        oss << "  " << cuboid.to_string() << std::endl;
    }
    oss << "]" << std::endl;

    return oss.str();
    }

    bool operator==(const ProgramArgs& other) const {


        if(cuboids.size() == other.cuboids.size() && spheres.size() == other.spheres.size()){
          auto cuboids_it = cuboids.begin();
          auto cuboids_it2 = other.cuboids.begin();
          

          for(; cuboids_it != cuboids.end() && cuboids_it2 != other.cuboids.end() ; ++cuboids_it , ++cuboids_it2){
            if( !(*cuboids_it ==  * cuboids_it2)){
                std::cout << "Comp failed bc these two were not equal:" << (*cuboids_it).to_string() << (*cuboids_it2).to_string() << std::endl;
                return false;
            }
          }

          auto spheres_it = spheres.begin();
          auto spheres_it2 = other.spheres.begin();

          for(;spheres_it != spheres.end() && spheres_it2 != other.spheres.end(); ++spheres_it , ++spheres_it2){
            if(!(*spheres_it == *spheres_it2)){
              std::cout << "Comp failed bc these two were not equal:" << (*spheres_it).to_string() << (*spheres_it2).to_string() << std::endl;
                return false;
            }
          }
        }else{
          std::cout << "Comp failed bc the structs had a different amount of cuboids or spheres" << std::endl;
          return false;
        }

        
        return (delta_t == other.delta_t &&
                t_end == other.t_end &&
                cut_of_radius == other.cut_of_radius &&
                cell_size == other.cell_size &&
                boundaries == other.boundaries &&
                domain_dimensions == other.domain_dimensions &&
                file_basename == other.file_basename &&
                write_frequency == other.write_frequency &&
                cuboids == other.cuboids &&
                spheres == other.spheres);
        
    }




  };


  /**
   * @brief read all program arguments
   * 
   * Reads all the Arguments that are specified within the ProgramArgs struct.
   * That means delta_t and end_t for the simulation, how often output files
   * should be written, the basename of these files and lastly an arbitrary
   * amount of cuboids and then an arbitrary amount of spheres. This data
   * should be specified in a certain xsd format (that is specified within 
   * 'project_root_dir'/input/parameters.xsd) and is then read into a struct that
   * is returned
   * 
   * 
   * @param filename name from which the arguments are read (has to be in xml format)
   * 
   * @return struct contains all necessary info for the program
   * 
  */
  ProgramArgs readProgramArguments(std::string filename);

  /**
   * @brief Reads particle data from a file and adds them into a
   * ParticleContainer
   *
   * reads particle data from a the file specified by filename(file has to have
   * specific format) and adds them inplace into the particleContainer
   *
   *
   * @param particleContainer reference to the ParticleContainer to add to
   * @param filename Filename of the file containing particle data
   */
  void readParticleFile(ParticleContainer &particleContainer, char *filename);

  /**
   * @brief Reads Cuboids of Particles from a file and returns a list of
   * CuboidData structs
   *
   * reads cuboids from a file specified by the given filename(file has to have
   * specific format). A cuboid in the file is just a list of the parameters
   * that are in the CuboidData struct. Turns the parameters given in the file
   * into a struct containing the parameters and creates a list of CuboidData
   * structs based on the read data.
   *
   * @param particleContainer reference to the ParticleContainer to add to
   * @param filename Filename of the file containing CuboidData
   *
   *
   * @return Returns a list of structs that contain the data of the Cuboids that
   * were read. Mainly used for testing and debugging purposes.
   *
   */
  std::list<CuboidData> readCuboidFile(std::string filename);
};
