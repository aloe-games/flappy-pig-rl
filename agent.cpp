#include <iostream>

#include "agent.hpp"

using namespace std;

double uniform() {
    return (double) rand() / RAND_MAX;
}

double maximum(double a, double b) {
    if (a > b) {
        return a;
    }
    return b;
}

double Agent::forward(double observation[2], bool action) {
    return observation[0] * W[action][0] + observation[1] * W[action][1] + b[action];
}

bool Agent::act(double observation[2]) {
    return forward(observation, 1) > forward(observation, 0);
}

void Agent::learn(double observation[2], bool action, double reward, double next_observation[2], bool done) {
    double Z = forward(observation, action);
    double Y = reward + (1 - done) * 0.99 * maximum(forward(next_observation, 0), forward(next_observation, 1));

    double dZ = Z - Y;
    double dW[2] = {observation[0] * dZ, observation[1] * dZ};
    double db = dZ;

    double alpha = 0.00005;
    W[action][0] -= alpha * dW[0];
    W[action][1] -= alpha * dW[1];
    b[action] -= alpha * db;
}