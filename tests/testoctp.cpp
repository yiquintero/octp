#include <gtest/gtest.h>
#include <iostream>

#define XSTR(x) STR(x)
#define STR(x) #x

class OctpTest : public testing::Test
{
public:
    OctpTest() {}
};

class TestEnvironment : public ::testing::Environment {
public:
    virtual void SetUp() { 
        std::cout << XSTR(LAMMPS_PATH) << std::endl;
    }
};


TEST_F(OctpTest, DummyTest) {
    EXPECT_TRUE(true);
}

TEST_F(OctpTest, DummyTest2) {
    EXPECT_TRUE(true);
}


int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);
  return RUN_ALL_TESTS();
}