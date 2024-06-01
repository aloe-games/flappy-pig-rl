#include <iostream>

#include "agent.hpp"

using namespace std;

const double Agent::EPS_START = 1.0;
const double Agent::EPS_DECAY = 0.995;
const double Agent::EPS_END = 0.01;

const double Agent::GAMMA = 0.99;

const double Agent::ALPHA = 0.0001;

double random_value() {
    return (double) rand() / RAND_MAX;
}

Agent::Agent() {
    W[0][0] = random_value();
    W[0][1] = random_value();
    W[1][0] = random_value();
    W[1][1] = random_value();
    b[0] = random_value();
    b[1] = random_value();
    step = 0;
    eps = Agent::EPS_START;
}

bool Agent::act(double observation[2]) {
    if (random_value() > eps) {
        double a[2];
        for (int i = 0; i < 2; i++) {
            a[i] = observation[0] * W[i][0] + observation[1] * W[i][1] + b[i];
            if (a[i] < 0) {
                a[i] = 0;
            }
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

    double A[2], Y[2], dA[2], dZ[2], dW[2][2], db[2];
    for (int i = 0; i < 2; i++) {
        cout << "O " << observation[0] << " " << observation[1] << endl;
        cout << "W " << W[i][0] << " " << W[i][1] << endl;
        cout << "b " << b[i] << endl;
        A[i] = observation[0] * W[i][0] + observation[1] * W[i][1] + b[i];
        cout << "Z " << A[i] << endl;
        if (A[i] < 0) {
            A[i] = 0;
        }
        if (i == action) {
            double next_max_rewards[2], next_max_reward;
            for (int j = 0; j < 2; j++) {
                next_max_rewards[j] = next_observation[0] * W[j][0] + next_observation[1] * W[j][1] + b[j];
                if (next_max_rewards[j] < 0) {
                    next_max_rewards[j] = 0;
                }
            }
            next_max_reward = next_max_rewards[0];
            if (next_max_rewards[1] > next_max_reward) {
                next_max_reward = next_max_rewards[1];
            }
            Y[i] = reward + (1 - done) * Agent::GAMMA * next_max_reward;

            cout << "Y " << Y[i] << endl;
            dA[i] = 2 * (A[i] - Y[i]);
            if (A[i] > 0) {
                dZ[i] = dA[i];
            } else {
                dZ[i] = 0;
            }
            dW[i][0] = observation[0] * dZ[i] + observation[1];
            dW[i][1] = observation[1] * dZ[i] + observation[0];
            db[i] = dZ[i];

            W[i][0] -= Agent::ALPHA * dW[i][0];
            W[i][1] -= Agent::ALPHA * dW[i][1];
            b[i] -= Agent::ALPHA * db[i];
        }
    }

    if (done) {
        step = 0;
    }
}