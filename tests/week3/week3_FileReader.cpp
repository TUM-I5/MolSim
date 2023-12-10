#include <gtest/gtest.h>
#include "inputHandling/FileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>


/**
 * @brief writes the struct programArgs into the file 'filename' in xml format
 * 
 * 
 * The function is a helper for the Test afterwards and writes the information
 * from the programArgs struct into an xml file according to a xsd schema
 * defined in parameters.xsd (is copied into build and test folder automatically)
 * 
 * @param programArgs struct containing info for the test
 * @param filename name of the file, in which the info is written
 * 
*/
void writeArgsIntoXMLFile(const FileReader::ProgramArgs &programArgs,std::string filename){
    // Construct the XML string
    std::ostringstream xmlStream;
    xmlStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    xmlStream << "<parameters xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
            "xsi:noNamespaceSchemaLocation=\"parameters.xsd\">" << std::endl;

    // Add outputParameters node under parameters
    xmlStream << "  <outputParameters>" << std::endl;
    xmlStream << "    <baseName>" << programArgs.file_basename << "</baseName>" << std::endl;
    xmlStream << "    <writeFrequency>" << programArgs.write_frequency << "</writeFrequency>" << std::endl;
    xmlStream << "  </outputParameters>" << std::endl;

    // Add simulationParameters node under parameters
    xmlStream << "  <simulationParameters>" << std::endl;
    xmlStream << "    <tEnd>" << programArgs.t_end << "</tEnd>" << std::endl;
    xmlStream << "    <deltaT>" << programArgs.delta_t << "</deltaT>" << std::endl;
    xmlStream << "    <cutOfRadius>" << programArgs.cut_of_radius << "</cutOfRadius>" << std::endl;
    xmlStream << "    <cellSize>" << programArgs.cell_size << "</cellSize>" << std::endl;
    xmlStream << "    <boundaryConditions>" << std::endl;
    xmlStream << "      <boundaryConditionsPositiveZ>" << (programArgs.boundaries[0] == boundary_conditions::reflective ?  "reflective" : "outflow") << "</boundaryConditionsPositiveZ> " << std::endl;
    xmlStream << "      <boundaryConditionsNegativeZ>" << (programArgs.boundaries[1] == boundary_conditions::reflective ?  "reflective" : "outflow") << "</boundaryConditionsNegativeZ> " << std::endl;
    xmlStream << "      <boundaryConditionsPositiveX>" << (programArgs.boundaries[2] == boundary_conditions::reflective ?  "reflective" : "outflow") << "</boundaryConditionsPositiveX> " << std::endl; 
    xmlStream << "      <boundaryConditionsNegativeX>" << (programArgs.boundaries[3] == boundary_conditions::reflective ?  "reflective" : "outflow") << "</boundaryConditionsNegativeX> " << std::endl; 
    xmlStream << "      <boundaryConditionsPositiveY>" << (programArgs.boundaries[4] == boundary_conditions::reflective ?  "reflective" : "outflow") << "</boundaryConditionsPositiveY> " << std::endl;
    xmlStream << "      <boundaryConditionsNegativeY>" << (programArgs.boundaries[5] == boundary_conditions::reflective ?  "reflective" : "outflow") << "</boundaryConditionsNegativeY> " << std::endl; 
    xmlStream << "</boundaryConditions>" << std::endl;
    xmlStream << "    <domainDimensions>" <<  std::endl;
    xmlStream << "      <x>" << programArgs.domain_dimensions[0] << "</x>" << std::endl;
    xmlStream << "      <y>" << programArgs.domain_dimensions[1] << "</y>" << std::endl;
    xmlStream << "      <z>" << programArgs.domain_dimensions[2] << "</z>" << std::endl;
    xmlStream << "    </domainDimensions>" << std::endl;
    xmlStream << "  </simulationParameters>" << std::endl;

    // Add cuboids under parameters
    for (const auto &cuboid : programArgs.cuboids) {
        xmlStream << "  <cuboids>" << std::endl;
        xmlStream << "    <position>" << std::endl;
        xmlStream << "      <x>" << cuboid.x[0] << "</x>" << std::endl;
        xmlStream << "      <y>" << cuboid.x[1] << "</y>" << std::endl;
        xmlStream << "      <z>" << cuboid.x[2] << "</z>" << std::endl;
        xmlStream << "    </position>" << std::endl;
        xmlStream << "    <velocity>" << std::endl;
        xmlStream << "      <x>" << cuboid.v[0] << "</x>" << std::endl;
        xmlStream << "      <y>" << cuboid.v[1] << "</y>" << std::endl;
        xmlStream << "      <z>" << cuboid.v[2] << "</z>" << std::endl;
        xmlStream << "    </velocity>" << std::endl;
        xmlStream << "    <dimensions>" << std::endl;
        xmlStream << "      <x>" << cuboid.N1 << "</x>" << std::endl;
        xmlStream << "      <y>" << cuboid.N2 << "</y>" << std::endl;
        xmlStream << "      <z>" << cuboid.N3 << "</z>" << std::endl;
        xmlStream << "    </dimensions>" << std::endl;
        xmlStream << "    <mass>" << cuboid.m << "</mass>" << std::endl;
        xmlStream << "    <meshWidth>" << cuboid.h << "</meshWidth>" << std::endl;
        xmlStream << "    <sigma>" << cuboid.sigma << "</sigma>" << std::endl;
        xmlStream << "    <epsilon>" << cuboid.epsilon << "</epsilon>" << std::endl;
        xmlStream << "  </cuboids>" << std::endl;
    }

    for (const auto &sphere : programArgs.spheres) {
        xmlStream << "  <spheres>" << std::endl;
        xmlStream << "    <center_position>" << std::endl;
        xmlStream << "      <x>" << sphere.CenterPosition[0] << "</x>" << std::endl;
        xmlStream << "      <y>" << sphere.CenterPosition[1] << "</y>" << std::endl;
        xmlStream << "      <z>" << sphere.CenterPosition[2] << "</z>" << std::endl;
        xmlStream << "    </center_position>" << std::endl;
        xmlStream << "    <velocity>" << std::endl;
        xmlStream << "      <x>" << sphere.Velocity[0] << "</x>" << std::endl;
        xmlStream << "      <y>" << sphere.Velocity[1] << "</y>" << std::endl;
        xmlStream << "      <z>" << sphere.Velocity[2] << "</z>" << std::endl;
        xmlStream << "    </velocity>" << std::endl;
        xmlStream << "    <mass>" << sphere.mass << "</mass>" << std::endl;
        xmlStream << "    <radius>" << sphere.radius << "</radius>" << std::endl;
        xmlStream << "    <meshWidth>" << sphere.meshWidth << "</meshWidth>" << std::endl;
        xmlStream << "    <sigma>" << sphere.sigma << "</sigma>" << std::endl;
        xmlStream << "    <epsilon>" << sphere.epsilon << "</epsilon>" << std::endl;
        xmlStream << "  </spheres>" << std::endl;
    }

    xmlStream << "</parameters>" << std::endl;

    // Write the XML string to a file
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << xmlStream.str();
        outputFile.close();
        std::cout << "XML file generated successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

}
    




/**
 * @brief Creates a dummy programArgs struct and writes it into a file "args_test.xml".
 *        Then the method from our project is used to again extract the information from the 
 *        xml file (the corresponding .xsd file has to be within the test folder as well,
 *        parameters.xsd should be copied into the /tests folder within the build folder 
 *        automatically). In the end the pre-defined struct and the struct gained 
 *        through writing + reading are compared for equality.
*/
TEST(test_readProgArgs,test_big){
    std::string filename = "args_test.xml";
    FileReader::SphereData sphere = {
        {1.5, 2.0, 3.0}, 
        {5.12, 3.343, 7.8}, 
        17.0,            
        2.5,             
        1.65,             
        0.5,             
        0.8              
    };

    // Initialize CuboidData with specific values
    FileReader::CuboidData cuboid = {
        {0.0, 23.2324, 9.9}, 
        {15.23, 1.435, 7.7}, 
        5,               
        9,               
        5,               
        20.0,            
        2.0,             
        0.3,             
        0.6,             
        0.1              
    };
    
    FileReader::ProgramArgs programArgs = {
        0.054,           // delta_t
        1422.0,          // t_end
        2.0,            //cut of radius
        1.0,            //cell size
        10.0,       //initial temp
        20.0,       //max temp diff
        30.0,       //target temp
        50,          //thermostat write frequency
        {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective
        },    //boundary conditions
        {5,5,5},        //domain size
        "out",          // file_basename
        10,             // write_frequency
        {cuboid},      // spheres
        {sphere}       // cuboids
    };


    FileReader fileReader;

    // write the information from the struct into
    // a xml file according to the defined xml format
    writeArgsIntoXMLFile(programArgs,filename);

    // let the method from the fileReader
    // read out the data from the previously created file
    FileReader::ProgramArgs programArgs_read = fileReader.readProgramArguments(filename);


    // check if the structs are equal
    // comparison operation is overloaded for the defined structs
    ASSERT_EQ(programArgs_read,programArgs);
}




