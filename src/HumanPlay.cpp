#include <iostream>
#include <memory>
#include <random>
#include <ctime>
#include <sys/time.h>
#include "Mcts.hpp"
#include "State.hpp"
#include "HumanPlay.hpp"

HumanPlay::HumanPlay(
    std::shared_ptr<Threads> pool,
    uint8_t threads,
    uint64_t timePerActionMillis,
    uint64_t maxIterations):
        mcts(std::make_unique<Mcts>(pool, threads, timePerActionMillis, maxIterations)) { }

uint8_t HumanPlay::play(State* state) {
    std::random_device rd;    
    std::mt19937 random(rd());
    std::uniform_int_distribution<uint8_t> uniform01(0,1);
    uint8_t c = uniform01(random);
    uint8_t player = 0;
    uint8_t action = -1;


    if (1+c%2 != 1) {
        state->print();
    }
    while (!state->isTerminal()) {
        player = 1 + c++ %2;
        if (player == 1) {
            struct timeval t1,t2;
            gettimeofday(&t1,NULL);
            std::cout << "Turn to AI" << std::endl;
            Mcts* mcts_ai = mcts.get();
            mcts_ai->setRoot(action, state);
            delete state;
            mcts_ai->think();
            state = mcts_ai->takeAction();
            action = mcts_ai->getLastAction();
            gettimeofday(&t2,NULL);
            double timeuse = t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
            //state->applyAction(action);
            state->print();
        } else {
            std::cout << "Turn to Human" << std::endl << "Please input row and col, such as 4 4" << std::endl;
            int x, y;
            std::cin >> x >> y;
            assert(x >= 0 && x<=14 && y>=0 && y <=14);
            state->applyAction(state->getDim() * x +y);
            state->print();
        }
    }

    bool draw = state->getWinner() == 0;
    if (draw){
        std::cout << "This is a draw game!" << std::endl;
    } else if (player == 1) {
        std::cout << "AI wins!" << std::endl;
    } else {
        std::cout << "You win!" << std::endl;
    }
    delete state;
    return !draw
        ? player
        : 0;
}

