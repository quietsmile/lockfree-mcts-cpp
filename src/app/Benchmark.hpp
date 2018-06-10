#pragma once

#include <cstdio>

class Benchmark {
public:

    Benchmark(int dim, int needed, int times, int maxIterations2);

    void run();
    void ai_pk();
    void human_pk();

private:

    std::array<uint16_t, 3> testScores(uint8_t threads1, uint32_t maxIterations1);

    const uint8_t numCpu;
    const uint16_t times;
    const uint32_t maxIterations2;
    const uint32_t timerPerActionSec{999000};
    const uint8_t dim;
    const uint8_t needed;

};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "specify: choice maxIterations\n";
        exit(1);
    }
    int choice = std::stoi(argv[1]);
    int dim = 15;
    int needed = 5;
    int times = 1;
    int maxIterations = std::stoi(argv[2]);
    Benchmark B = Benchmark(dim, needed, times, maxIterations);
    if (choice == 0) {
        B.human_pk();
    } else if(choice == 1) {
        std::cout << "ai_pk" << std::endl;
        B.ai_pk(); 
    } else {
        std::cout << "not supported" << std::endl;
    }
    //Benchmark(dim, needed, times, maxIterations).pk();
}


