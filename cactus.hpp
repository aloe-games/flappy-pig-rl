#ifndef _cactus_h
#define _cactus_h

#include <allegro5/allegro.h>

class Cactus {
public:
    static const int HEIGHT;
    static const int WIDTH;
    protected:
        int x;
        int top;
        int gap;
        ALLEGRO_BITMAP **bitmap;
    public:
        Cactus(int x = 0, int gap = 0, ALLEGRO_BITMAP **bitmap = NULL);
        int getX();
        int getTop();
        int getGap();
        int getBottom();
        void randomTop();
        void draw(int offset);
};

#endif
