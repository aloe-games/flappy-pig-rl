#ifndef _agent_h
#define _agent_h

class Agent {
    public:
        static const double EPS_START;
        static const double EPS_DECAY;
        static const double EPS_END;
    protected:
        double w[2][2];
        double b[2];
        int step;
        double eps;
    public:
        Agent();
        bool act(int observation[2]);
        void learn(int observation[2], bool action, int reward, int next_observation[2], bool done);
};

double random_value();

#endif
