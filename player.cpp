#include "player.h"

const int Player::size = 48;

Player::Player(int left, int top, ALLEGRO_BITMAP *bitmap) {
    this->left = left;
    this->top = top;
    this->bitmap = bitmap;
    this->score = 0;
}

int Player::getLeft() {
    return this->left;
}

int Player::getTop() {
    return this->top;
}

int Player::getRight() {
    return this->left + this->size;
}

int Player::getBottom() {
    return this->top + this->size;
}

int Player::getScore() {
    return this->score;
}

void Player::setLeft(int left) {
    this->left = left;
}

void Player::setTop(int top) {
    this->top = top;
}

void Player::incScore() {
    this->score++;
}

void Player::resetScore() {
    this->score = 0;
}

void Player::draw() {
    al_draw_bitmap(this->bitmap, this->left, this->top, 0);
}
