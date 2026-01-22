#include <iostream>
#include "tests/testmatrix.h"
#include "tests/testvector3.h"

int main()
{
    std::cout << "Running Vector3 tests" << std::endl;
    runVector3Tests();
    std::cout << "Running Matrix tests" << std::endl;
    runMatrixTests();
    return 0;
}
