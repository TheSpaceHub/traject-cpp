#include <iostream>
#include <conio.h>
#include "renderer.h"
#include "simulateTrajectory.h"
#include "solveTrajectory.h"

void displayTitle()
{
    std::cout << "Traject-cpp\n\n";
    std::cout << "C++ trajectory simulator, solver and visualizer\n\n";
    std::cout << "1: Simulate a trajectory specifying the parameters\n";
    std::cout << "2: Find the optimal trajectory given certain parameters\n";
    std::cout << "Select functionality: ";
}

void functionalityccin(int &d, int retriesLeft = 3)
{
    // ccin retries the cin if the user makes a mistake
    try
    {
        std::cin >> d;
        if (d != 1 and d != 2)
            throw std::exception();
    }
    catch (...)
    {
        std::cout << "Check the input and try again.\nSelect functionality: ";
        functionalityccin(d, retriesLeft - 1);
    }
}

int main()
{

    displayTitle();
    int functionality;
    functionalityccin(functionality);

    switch (functionality)
    {
    case 1:
        simulateTrajectory();
        break;
    case 2:
        solveTrajectory();
        break;

    default:
        break;
    }

    std::cout << "\nPress any key to end program...";
    _getch();
    return 0;
}
