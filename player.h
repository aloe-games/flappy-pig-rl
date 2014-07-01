#ifndef __player_h
#define __player_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

class Player {
    public:
        static const int SIZE;
        static const float DEFAULT_DX;
        static const float DEFAULT_DY;
    protected:
        int left;
        int top;
        ALLEGRO_BITMAP *bitmap;
        ALLEGRO_SAMPLE *sound;
        int score;
        float dx;
        float dy;
    
    public:
        Player(int top, int left, ALLEGRO_BITMAP *bitmap, ALLEGRO_SAMPLE *sound);
        int getLeft();
        int getTop();
        int getRight();
        int getBottom();
        int getSize();
        int getScore();
        float getDx();
        float getDy();
        void setLeft(int left);
        void setTop(int top);
        void incScore();
        void resetScore();
        void resetSpeed();
        void jump();
        void timer();
        void draw(int offset);
};

#endif
