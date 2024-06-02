#ifndef _agent_h
#define _agent_h

double uniform();

double maximum(double a, double b);

class Agent {
protected:
    double W[2][2] = {{uniform(), uniform()},
                      {uniform(), uniform()}};
    double b[2] = {uniform(), uniform()};
public:
    double forward(double observation[2], bool action);

    bool act(double observation[2]);

    void learn(double observation[2], bool action, double reward, double next_observation[2], bool done);
};

#endif
