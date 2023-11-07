#include <iostream>
#include "Tests.h"

bool Tests::SampleTest() {
    return 1 == 0;
}

void Tests::RunTests()
{
    //Run a single test
    std::cout << "Sample Test: " << (SampleTest() ? "Passed" : "Failed") << std::endl;
}