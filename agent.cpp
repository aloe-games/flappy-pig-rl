#include <iostream>

#include "agent.hpp"

using namespace std;

const double Agent::EPS_START = 1.0;
const double Agent::EPS_DECAY = 0.995;
const double Agent::EPS_END = 0.01;

Agent::Agent() {
    w[0] = 0.0;
    w[1] = -1.0;
    b = 10.0;
    step = 0;
}

bool Agent::act(int observation[2]) {
    return (observation[0] * w[0] + observation[1] * w[1] + b) > 0.0;
}

void Agent::learn(int observation[2], bool action, int reward, int next_observation[2], bool done) {
    cout << "step: " << step;
    cout << ", observation: [" << observation[0] << ", " << observation[1] << "]";
    cout << ", action: " << action;
    cout << ", reward: " << reward;
    cout << ", next_observation: [" << next_observation[0] << ", " << next_observation[1] << "]";
    cout << ", done: " << done;
    cout << endl;

    step++;
    if (done) {
        step = 0;
    }
}