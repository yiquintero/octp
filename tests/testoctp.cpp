#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#define XSTR(x) STR(x)
#define STR(x) #x

std::string getLAMMPSPath() {
    std::stringstream buf;
    buf << XSTR(LAMMPS_PATH);
    return buf.str();
}

std::string getTestDataPath() {
    std::stringstream buf;
    buf << XSTR(TEST_DATA_PATH);
    return buf.str();
}

class OctpTest : public testing::Test {
public:
    OctpTest()
    : testdatapath(getTestDataPath()), lammpspath(getLAMMPSPath())
    {}

    const std::string testdatapath;
    const std::string lammpspath;
};

class TestEnvironment : public ::testing::Environment {
public:
    virtual void SetUp() { 
        // Run LAMMPS + octp to generate testing data that will be compared to gold standard
        // 1. open LAMMPS dir
        std::string cmnd = "cd " + getLAMMPSPath();
        // 2. copy simulation input files
        cmnd.append(" && cp --verbose " + getTestDataPath() + "gold-standard/simulation.in .");
        cmnd.append(" && cp --verbose " + getTestDataPath() + "gold-standard/data.lmp .");
        cmnd.append(" && cp --verbose " + getTestDataPath() + "gold-standard/forcefield.data .");
        // 3. run simulation
        cmnd.append(" && echo '...Running LAMMPS simulation...' && mpirun -n 8 lmp_mpi -in simulation.in > simoutput.txt");
        // 4. copy output files to octp/tests/data/
        std::vector<std::string> simulation_output_files = {
            "diffonsag.dat",
            "diffself.dat",
            "log.lammps",
            "PotenE.dat",
            "pressure.dat",
            "rdf.dat",
            "tconductivity.dat",
            "temperature.dat",
            "TotalE.dat",
            "viscosity.dat",
            "simoutput.txt"
        };
        for (std::string file : simulation_output_files){
            std::cout << file << std::endl;
            cmnd.append(" && cp --verbose " + file + " " + getTestDataPath());
        }
        // run commands
        std::system(cmnd.c_str());
    }

};


TEST_F(OctpTest, DummyTest) {
    std::cout << "dumdum1 " + lammpspath << std::endl << testdatapath << std::endl;
    EXPECT_TRUE(true);
}

TEST_F(OctpTest, DummyTest2) {
    std::cout << "dumdum2 " + lammpspath << std::endl << testdatapath << std::endl;
    EXPECT_TRUE(true);
}


int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);
  return RUN_ALL_TESTS();
}