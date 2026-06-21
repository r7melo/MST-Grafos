#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <functional>

using namespace std;

class Timer {
public:
    // Recebe a variável de saída por referência (&) e o bloco de código
    static void calculateTime(double& outputTime, function<void()> code) {
        // Inicia o cronometro
        auto startTime = chrono::high_resolution_clock::now();

        // Executa o codigo a ser calculado
        code();

        // Desliga o cronometro
        auto endTime = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> dtime = endTime - startTime;
        
        // Modifica diretamente o valor da variável que foi passada por parâmetro
        outputTime = dtime.count();
    }
};

#endif // TIMER_H