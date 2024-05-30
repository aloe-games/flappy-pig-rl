#ifndef _agent_h
#define _agent_h

class Agent {
    protected:
        float w[2];
        float b;
    public:
        Agent();
        bool act(int observation[2]);
        void learn(int observation[2], bool action, int reward, int next_observation[2], bool done);
};

#endif
