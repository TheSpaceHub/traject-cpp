#include <iostream>
#include "tests/testmatrix.h"
#include "tests/testvector3.h"
#include "tests/testrenderer.h"
#include "tests/testrk4.h"

int main()
{
    std::cout << "Running Vector3 tests" << std::endl;
    runVector3Tests();
    std::cout << "Running Matrix tests" << std::endl;
    runMatrixTests();
    /*std::cout << "Running Renderer tests" << std::endl;
    runRendererTests();*/
    std::cout << "Running RK4 tests" << std::endl;
    runRK4Tests();
    return 0;
}
