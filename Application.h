#pragma once
#include "Fork.h"
#include "Philosopher.h"
#include <vector>
#include <thread>
#include <memory>

// Clase principal que arma la mesa: crea los tenedores, los filosofos
// y los hilos, y coordina el inicio/fin de la simulacion.
class Application {
public:
    Application(int numPhilosophers, int numCycles);

    void run();

private:
    int numPhilosophers;
    int numCycles;

    // Se usan punteros/unique_ptr porque Fork y Philosopher no son copiables
    // (Fork tiene un mutex, Philosopher guarda referencias).
    std::vector<std::unique_ptr<Fork>> forks;
    std::vector<std::unique_ptr<Philosopher>> philosophers;
    std::vector<std::thread> threads;
};
