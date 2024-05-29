#ifndef _cactus_h
#define _cactus_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

class Cactus {
    public:
        static const int HEIGHT;
        static const int WIDTH;
    protected:
        int x;
        int top;
        int gap;
        ALLEGRO_BITMAP **bitmap;
        ALLEGRO_SAMPLE *sound;
    public:
        Cactus(int x = 0, int gap = 0, ALLEGRO_BITMAP **bitmap = NULL, ALLEGRO_SAMPLE *sound = NULL);
        int getX();
        int getTop();
        int getGap();
        int getBottom();
        void draw(int offset);
};

#endif
