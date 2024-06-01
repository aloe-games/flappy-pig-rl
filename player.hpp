#ifndef _player_h
#define _player_h

#include <allegro5/allegro.h>

class Player {
public:
    static const int SIZE;
    static const double DEFAULT_DX;
    static const double DEFAULT_DY;
protected:
    int left;
    int top;
    ALLEGRO_BITMAP *bitmap;
    int score;
    double dx;
    double dy;

public:
    Player(int top, int left, ALLEGRO_BITMAP *bitmap);

    int getLeft();

    int getTop();

    int getRight();

    int getBottom();

    int getScore();

    double getDx();

    double getDy();

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
