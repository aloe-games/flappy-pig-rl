#ifndef _agent_h
#define _agent_h

class Agent {
    public:
        static const double EPS_START;
        static const double EPS_DECAY;
        static const double EPS_END;
        static const double GAMMA;
        static const double ALPHA;
    protected:
        double W[2][2];
        double b[2];
        int step;
        double eps;
    public:
        Agent();
        bool act(double observation[2]);
        void learn(double observation[2], bool action, double reward, double next_observation[2], bool done);
};

double uniform();
double maximum(double a, double b);
double relu(double x);

#endif
