#include "Philosopher.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

std::mutex coutMutex;

Philosopher::Philosopher(int id, Fork& leftFork, Fork& rightFork)
    : id(id), leftFork(leftFork), rightFork(rightFork) {}

int Philosopher::getId() const {
    return id;
}

void Philosopher::log(const std::string& mensaje) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[Filosofo " << id << "] " << mensaje << std::endl;
}

void Philosopher::think() {
    log("esta pensando...");
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(100, 400);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
}

void Philosopher::eat() {
    // Estrategia anti-interbloqueo: los filosofos con id par toman
    // primero el tenedor izquierdo y luego el derecho; los impares
    // hacen lo contrario. Esto rompe la espera circular clasica.
    Fork& primerTenedor = (id % 2 == 0) ? leftFork : rightFork;
    Fork& segundoTenedor = (id % 2 == 0) ? rightFork : leftFork;

    log("tiene hambre y quiere comer");

    primerTenedor.pickUp();
    log("tomo el primer tenedor");

    segundoTenedor.pickUp();
    log("tomo el segundo tenedor y esta comiendo");

    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(100, 300);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));

    segundoTenedor.putDown();
    primerTenedor.putDown();

    log("termino de comer y solto ambos tenedores");
}

void Philosopher::dine(int numCycles) {
    for (int i = 0; i < numCycles; ++i) {
        think();
        eat();
    }
    log("ha terminado la cena (todos sus ciclos)");
}
