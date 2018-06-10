#pragma once

#include "Mcts.hpp"

class State;
class Threads;

class HumanPlay {
public:

    HumanPlay(
        std::shared_ptr<Threads> pool,
        uint8_t threads, 
        uint64_t timePerActionMillis, 
        uint64_t maxIterations);


    uint8_t play(State* state);

private:

    std::unique_ptr<Mcts> mcts;

};

