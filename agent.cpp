#include <iostream>

#include "agent.hpp"

using namespace std;

const double Agent::EPS_START = 1.0;
const double Agent::EPS_DECAY = 0.995;
const double Agent::EPS_END = 0.01;

double random_value() {
    return (double) rand() / RAND_MAX;
}

Agent::Agent() {
    w[0][0] = random_value();
    w[0][1] = random_value();
    w[1][0] = random_value();
    w[1][1] = random_value();
    b[0] = random_value();
    b[1] = random_value();
    step = 0;
    eps = Agent::EPS_START;
}

bool Agent::act(int observation[2]) {
    if (random_value() > eps) {
        double a[2];
        for (int i = 0; i < 2; i++) {
            a[i] = (observation[0] * w[i][0] + observation[1] * w[i][1] + b[i]) > 0.0;
        }
        return int(a[1] > a[0]);
    }
    return rand() % 2;
}

void Agent::learn(int observation[2], bool action, int reward, int next_observation[2], bool done) {
    cout << "step: " << step;
    cout << ", epsilon: " << eps;
    cout << ", observation: [" << observation[0] << ", " << observation[1] << "]";
    cout << ", action: " << action;
    cout << ", reward: " << reward;
    cout << ", next_observation: [" << next_observation[0] << ", " << next_observation[1] << "]";
    cout << ", done: " << done;
    cout << endl;

    step++;
    eps = max(eps * Agent::EPS_DECAY, Agent::EPS_END);
    if (done) {
        step = 0;
    }
}