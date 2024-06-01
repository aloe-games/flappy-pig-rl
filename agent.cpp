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

    // learn
    double a[2], y[2], loss[2], dA[2], dZ[2], dW[2][2], dB[2];
    for (int i = 0; i < 2; i++) {
        a[i] = (observation[0] * w[i][0] + observation[1] * w[i][1] + b[i]) > 0.0;
        if (i == action) {
            double next_max_rewards[2], next_max_reward;
            for (int j = 0; j < 2; j++) {
                next_max_rewards[j] = (observation[0] * w[j][0] + observation[1] * w[j][1] + b[j]) > 0.0;
            }
            next_max_reward = next_max_rewards[0];
            if (next_max_rewards[1] > next_max_reward) {
                next_max_reward = next_max_rewards[1];
            }
            y[i] = reward + (1 - done) * 0.99 * next_max_reward;
        } else {
            y[i] = a[i];
        }
        loss[i] = pow(a[i] - y[i], 2);
        dA[i] = 2 * (a[i] - y[i]);
        if (a[i] > 0) {
            dZ[i] = dA[i];
        } else {
            dZ[i] = 0;
        }
        dW[i][0] = observation[1] * dZ[i];
        dW[i][1] = observation[0] * dZ[i];
        dB[i] = dZ[i];

        w[i][0] -= 0.001 * dW[i][0];
        w[i][1] -= 0.001 * dW[i][1];
        b[i] -= 0.001;
    }

    if (done) {
        step = 0;
    }
}