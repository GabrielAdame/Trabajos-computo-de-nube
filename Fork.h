#pragma once
#include <mutex>

// Representa un tenedor compartido entre dos filosofos.
// Internamente usa un std::mutex para garantizar acceso exclusivo.
class Fork {
public:
    Fork() = default;

    // No se permite copiar (un mutex no es copiable)
    Fork(const Fork&) = delete;
    Fork& operator=(const Fork&) = delete;

    void pickUp();
    void putDown();

private:
    std::mutex mtx;
};
