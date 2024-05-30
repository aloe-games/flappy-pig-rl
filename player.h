#ifndef _player_h
#define _player_h

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
        int getScore();
        float getDx();
        float getDy();
        void setLeft(int left);
        void setTop(int top);
        void incScore();
        void resetScore();
        void resetSpeed();
        void jump(bool sound);
        void timer();
        void draw(int offset);
};

#endif
