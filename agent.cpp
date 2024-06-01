#include <iostream>

#include "agent.hpp"

using namespace std;

const double Agent::GAMMA = 0.99;
const double Agent::ALPHA = 0.0001;

double uniform() {
    return (double) rand() / RAND_MAX;
}

double maximum(double a, double b) {
    if (a > b) {
        return a;
    }
    return b;
}

double relu(double x) {
    return maximum(x, 0);
}

double Agent::forward(double observation[2], bool action) {
    return relu(observation[0] * W[action][0] + observation[1] * W[action][1] + b[action]);
}

bool Agent::act(double observation[2]) {
    return forward(observation, 1) > forward(observation, 0);
}

void Agent::learn(double observation[2], bool action, double reward, double next_observation[2], bool done) {
    double A, Y, dA, dZ, dW[2], db;

    A = forward(observation, action);
    Y = reward + (1 - done) * Agent::GAMMA * maximum(forward(next_observation, 0), forward(next_observation, 1));

    dA = 2 * (A - Y);
    dZ = dA * int(A > 0);
    dW[0] = observation[0] * dZ;
    dW[1] = observation[1] * dZ;
    db = dZ;

    W[action][0] -= Agent::ALPHA * dW[0];
    W[action][1] -= Agent::ALPHA * dW[1];
    b[action] -= Agent::ALPHA * db;
}