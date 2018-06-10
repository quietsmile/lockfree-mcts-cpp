#pragma once

#include <vector>
#include <memory>

class State {
public:
    virtual ~State() {};

    virtual std::vector<uint16_t>* getAvailableActions() = 0;

    virtual bool isTerminal() = 0;

    virtual uint8_t getPreviousAgent() = 0;

    virtual uint8_t getRewardFor(uint8_t agent) = 0;

    virtual State* takeAction(uint16_t action) = 0;

    virtual void applyAction(uint16_t action) = 0;

    virtual uint8_t getWinner() = 0;

    virtual State* copy() = 0;

    virtual void print() = 0;

    virtual uint8_t getDim() = 0;

    virtual int16_t getLastAction() = 0;
};
