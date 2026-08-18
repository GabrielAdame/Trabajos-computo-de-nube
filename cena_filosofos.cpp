// =====================================================================
//  La Cena de los Filosofos - version en un solo archivo
//  Clases: Fork, Philosopher, Application (MainApplication)
// =====================================================================

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>
#include <random>
#include <string>
#include <cstdlib>

// ---------------------------------------------------------------------
// Mutex global solo para que los mensajes en consola no se entremezclen
// ---------------------------------------------------------------------
std::mutex coutMutex;

// =====================================================================
// Clase Fork
// Representa un tenedor compartido entre dos filosofos.
// Internamente usa un std::mutex para garantizar acceso exclusivo.
// =====================================================================
class Fork {
public:
    Fork() = default;

    // No se permite copiar (un mutex no es copiable)
    Fork(const Fork&) = delete;
    Fork& operator=(const Fork&) = delete;

    void pickUp() {
        mtx.lock();
    }

    void putDown() {
        mtx.unlock();
    }

private:
    std::mutex mtx;
};

// =====================================================================
// Clase Philosopher
// Representa a un filosofo: piensa, tiene hambre, come, y repite.
// =====================================================================
class Philosopher {
public:
    Philosopher(int id, Fork& leftFork, Fork& rightFork)
        : id(id), leftFork(leftFork), rightFork(rightFork) {}

    // Ejecuta el ciclo pensar -> comer durante 'numCycles' repeticiones
    void dine(int numCycles) {
        for (int i = 0; i < numCycles; ++i) {
            think();
            eat();
        }
        log("ha terminado la cena (todos sus ciclos)");
    }

    int getId() const { return id; }

private:
    int id;
    Fork& leftFork;
    Fork& rightFork;

    void log(const std::string& mensaje) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "[Filosofo " << id << "] " << mensaje << std::endl;
    }

    void think() {
        log("esta pensando...");
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(100, 400);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
    }

    void eat() {
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
};

// =====================================================================
// Clase Application (MainApplication)
// Arma la mesa: crea los tenedores, los filosofos y los hilos, y
// coordina el inicio/fin de la simulacion.
// =====================================================================
class Application {
public:
    Application(int numPhilosophers, int numCycles)
        : numPhilosophers(numPhilosophers), numCycles(numCycles) {

        // Crear un tenedor por filosofo (mesa circular)
        for (int i = 0; i < numPhilosophers; ++i) {
            forks.push_back(std::make_unique<Fork>());
        }

        // Cada filosofo i usa el tenedor i (izquierdo) y el tenedor
        // (i+1)%N (derecho)
        for (int i = 0; i < numPhilosophers; ++i) {
            Fork& left = *forks[i];
            Fork& right = *forks[(i + 1) % numPhilosophers];
            philosophers.push_back(std::make_unique<Philosopher>(i, left, right));
        }
    }

    void run() {
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

private:
    int numPhilosophers;
    int numCycles;

    std::vector<std::unique_ptr<Fork>> forks;
    std::vector<std::unique_ptr<Philosopher>> philosophers;
    std::vector<std::thread> threads;
};

// =====================================================================
// main
// =====================================================================
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
