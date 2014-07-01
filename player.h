#ifndef __player_h
#define __player_h

#include <allegro5/allegro.h>

class Player {
    public:
        static const int size;
    protected:
        int left;
        int top;
        ALLEGRO_BITMAP *bitmap;
        int score;
    
    public:
        Player(int top, int left, ALLEGRO_BITMAP *bitmap);
        int getLeft();
        int getTop();
        int getRight();
        int getBottom();
        int getScore();
        void setLeft(int left);
        void setTop(int top);
        void incScore();
        void resetScore();
        void draw();
};

#endif
