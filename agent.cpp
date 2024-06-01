#include <iostream>

#include "agent.hpp"

using namespace std;

const double Agent::EPS_START = 1.0;
const double Agent::EPS_DECAY = 0.995;
const double Agent::EPS_END = 0.01;

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

Agent::Agent() {
    W[0][0] = uniform();
    W[0][1] = uniform();
    W[1][0] = uniform();
    W[1][1] = uniform();
    b[0] = uniform();
    b[1] = uniform();
    step = 0;
    eps = Agent::EPS_START;
}

bool Agent::act(double observation[2]) {
    if (uniform() > eps) {
        double a[2];
        for (int i = 0; i < 2; i++) {
            a[i] = relu(observation[0] * W[i][0] + observation[1] * W[i][1] + b[i]);
        }
        return int(a[1] > a[0]);
    }
    return rand() % 2;
}

void Agent::learn(double observation[2], bool action, double reward, double next_observation[2], bool done) {
    step++;

    if (step % 5 == 0) {
        eps = max(eps * Agent::EPS_DECAY, Agent::EPS_END);
    }

    double A, Y, dA, dZ, dW[2], db;
    A = relu(observation[0] * W[action][0] + observation[1] * W[action][1] + b[action]);
    Y = reward + (1 - int(done)) * Agent::GAMMA * maximum(
        relu(next_observation[0] * W[0][0] + next_observation[1] * W[0][1] + b[0]),
        relu(next_observation[0] * W[1][0] + next_observation[1] * W[1][1] + b[1])
    );

    dA = 2 * (A - Y);
    dZ = dA * int(A > 0);
    dW[0] = observation[0] * dZ;
    dW[1] = observation[1] * dZ;
    db = dZ;

    W[action][0] -= Agent::ALPHA * dW[0];
    W[action][1] -= Agent::ALPHA * dW[1];
    b[action] -= Agent::ALPHA * db;

    if (done) {
        step = 0;
    }
}