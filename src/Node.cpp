#include <atomic>
#include <vector>
#include <cmath>
#include <cassert>
#include <limits>
#include <iostream>

#include "Node.hpp"
#include "State.hpp"

        
Node::Node(Node* parent, State* state):
    parent(parent),
    action(action),
    state(state),
    prior_prob(1.0),
    untakenActions(state->getAvailableActions()),
    untakenIndex(untakenActions->size() - 1) {

    if (!state) throw std::logic_error("state null!");

    children = new std::atomic<Node*>[untakenActions->size()];

    for (size_t i = 0; i < untakenActions->size(); i++) {
        children[i] = NULL;
    }
}

Node::~Node() {
    delete state;
    for (size_t i = 0; i < untakenActions->size(); i++) {
        if (children[i].load() != NULL) delete children[i].load();
    }
    delete[] children;
    delete untakenActions;
}

void Node::releaseParent() {
    parent = NULL;
}

void Node::deleteChildrenExcept(Node* except) {
    bool found = false;
    for (size_t i = 0; i < untakenActions->size(); i++) {
        if (children[i].load() == except) {
            children[i] = NULL;
            found = true;
            continue;
        }
        delete children[i].load();
        children[i] = NULL;
    }
    if (!found) throw std::logic_error("not found child");
}

Node* Node::findChildFor(uint16_t action) {
    for (size_t i = 0; i < untakenActions->size(); i++) {
        if (!children[i].load()) continue;
        if (children[i].load()->action == action) return children[i].load();
    }
    return NULL;
}

uint16_t Node::getAction() {
    return action;
}

bool Node::isTerminal() {
    return state->isTerminal();
}

Node* Node::expand() {
    if (untakenIndex.load()<0) return NULL;
    int16_t untaken = untakenIndex.fetch_sub(1);
    if (untaken < 0) return NULL;

    uint16_t untakenAction = untakenActions->at(untaken);
    State* actionState = state->takeAction(untakenAction);
    Node* node = new Node(this, untakenAction, actionState);
    children[untaken] = node;
    return node;
}

Node* Node::getParent() {
    return parent;
}

uint8_t Node::getPreviousAgent() {
    return state->getPreviousAgent();
}

void Node::expand(net_result) {
    auto actions = state->getAvailableActions()
    for (int i = 0; i < actions.size(); i++) {
    //for (auto action: actions) {
        auto action = actions[i];
        prob = net_result[action];
        State* actionState = state->takeAction(action);
        Node* node = new Node(this, action, actionState);
        node->setPriorProb(prob);
        children[i] = node;
    }
}

void Node::update(float reward) {
    _n_visits ++;
    _W += reward;
    _Q = _W / _n_visits;
}
void Node::update_recursive(float reward) {
   if (parent) {
       parent->update_recursive(-reward);
   }
   update(reward);
}

bool Node::isVisited() {
    return visits.load() > 0;
}

double Node::getUctValue(double c) {
    int visits1 = visits.load();
    //return (double) rewards.load() / visits1 + c * std::sqrt(std::log(parent->visits.load()) / visits1);
    return (double) rewards.load() / visits1 + c * prior_prob * std::sqrt(parent->visits.load()) / (1. + visits1);
}

Node* Node::childToExploit() {
    return getBestChild(NO_EXPLORATION);
}

Node* Node::childToExplore() {
    return getBestChild(EXPLORATION_CONSTANT);
}

Node* Node::getBestChild(double c) {
    Node* best = NULL;
    while (!best) {
        double bestValue = std::numeric_limits<double>::lowest();
        for (size_t i = 0; i < untakenActions->size(); i++) {
            while (!children[i].load()) {};
            while (!children[i].load()->isVisited()) {};

            double childrenValue = children[i].load()->getUctValue(c);
            if (childrenValue > bestValue) {
                best = children[i].load();
                bestValue = childrenValue;
            }
        }
    }
    return best;
}

State* Node::getStateCopy() {
    return state->copy();
}


