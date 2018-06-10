#include <iostream>
#include <memory>
#include <random>
#include "Mcts.hpp"
#include "State.hpp"
#include "SelfPlay.hpp"

SelfPlay::SelfPlay(
    std::shared_ptr<Threads> pool1, std::shared_ptr<Threads> pool2,
    uint8_t threads1, uint8_t threads2,
    uint64_t timePerActionMillis1, uint64_t timePerActionMillis2,
    uint64_t maxIterations1, uint64_t maxIterations2):
        mcts1(std::make_unique<Mcts>(pool1, threads1, timePerActionMillis1, maxIterations1)),
        mcts2(std::make_unique<Mcts>(pool2, threads2, timePerActionMillis2, maxIterations2)) { }

uint8_t SelfPlay::play(State* state) {
    std::random_device rd;    
    std::mt19937 random(rd());
    std::uniform_int_distribution<uint8_t> uniform01(0,1);
    uint8_t c = uniform01(random);
    uint8_t player = 0;
    uint8_t action = -1;
    while (!state->isTerminal()) {
        player = 1 + c++ %2;
        Mcts* mcts = player == 1
            ? mcts1.get() 
            : mcts2.get();
        mcts->setRoot(action, state);
        delete state;
        mcts->think();

        state = mcts->takeAction();
        action = mcts->getLastAction();
        state->print();
    }
//state->print();
    bool draw = state->getWinner() == 0;
    delete state;
    return !draw
        ? player
        : 0;
}

