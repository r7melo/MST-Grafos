#include <iostream>
#include <chrono>
#include <functional>

using namespace std;

class Timer {
private:
    static inline double lastTime = 0;

public:
    // Recebe o codigo que sera executado seu tempo
    static void calculateTime(function<void()> code) {
        // Inicia o cronometro
        auto startTime = chrono::high_resolution_clock::now();

        // Executa o codigo a ser calculado
        code();

        // Desliga o cronometro
        auto endTime = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> dtime = endTime - startTime;
        lastTime = dtime.count();
    }

    // Resgatar o tempo registrado
    static double getTime() {
        return lastTime;
    }
};