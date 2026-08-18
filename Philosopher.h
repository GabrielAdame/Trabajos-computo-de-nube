#pragma once
#include "Fork.h"
#include <string>
#include <mutex>

// Mutex global solo para que los mensajes en consola no se entremezclen
extern std::mutex coutMutex;

// Representa a un filosofo: piensa, tiene hambre, come, y repite.
class Philosopher {
public:
    Philosopher(int id, Fork& leftFork, Fork& rightFork);

    // Ejecuta el ciclo pensar -> comer durante 'numCycles' repeticiones
    void dine(int numCycles);

    int getId() const;

private:
    int id;
    Fork& leftFork;
    Fork& rightFork;

    void think();
    void eat();
    void log(const std::string& mensaje);
};
