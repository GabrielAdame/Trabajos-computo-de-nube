#include "Application.h"
#include <iostream>

int main(int argc, char* argv[]) {
    int numPhilosophers = 5;  // por defecto: 5 filosofos (el clasico)
    int numCycles = 3;        // por defecto: 3 ciclos de pensar/comer cada uno

    if (argc >= 2) {
        numPhilosophers = std::atoi(argv[1]);
    }
    if (argc >= 3) {
        numCycles = std::atoi(argv[2]);
    }

    if (numPhilosophers < 2) {
        std::cerr << "Se necesitan al menos 2 filosofos." << std::endl;
        return 1;
    }

    Application app(numPhilosophers, numCycles);
    app.run();

    return 0;
}
