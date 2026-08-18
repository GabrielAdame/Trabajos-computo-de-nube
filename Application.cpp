#include "Application.h"
#include <iostream>

Application::Application(int numPhilosophers, int numCycles)
    : numPhilosophers(numPhilosophers), numCycles(numCycles) {

    // Crear un tenedor por filosofo (mesa circular)
    for (int i = 0; i < numPhilosophers; ++i) {
        forks.push_back(std::make_unique<Fork>());
    }

    // Cada filosofo i usa el tenedor i (izquierdo) y el tenedor (i+1)%N (derecho)
    for (int i = 0; i < numPhilosophers; ++i) {
        Fork& left = *forks[i];
        Fork& right = *forks[(i + 1) % numPhilosophers];
        philosophers.push_back(std::make_unique<Philosopher>(i, left, right));
    }
}

void Application::run() {
    std::cout << "=== Iniciando la Cena de los Filosofos ===" << std::endl;
    std::cout << "Numero de filosofos: " << numPhilosophers << std::endl;
    std::cout << "Ciclos por filosofo: " << numCycles << std::endl;
    std::cout << "============================================" << std::endl;

    // Lanzar un hilo por filosofo
    for (auto& philosopher : philosophers) {
        threads.emplace_back(&Philosopher::dine, philosopher.get(), numCycles);
    }

    // Esperar a que todos terminen
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "============================================" << std::endl;
    std::cout << "=== La cena ha finalizado. Nadie murio de hambre. ===" << std::endl;
}
