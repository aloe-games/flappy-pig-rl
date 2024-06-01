#include "player.hpp"

const int Player::SIZE = 48;
const double Player::DEFAULT_DX = 3.0;
const double Player::DEFAULT_DY = 2.0;

Player::Player(int left, int top, ALLEGRO_BITMAP *bitmap) {
    this->left = left;
    this->top = top;
    this->bitmap = bitmap;
    this->score = 0;
    this->dx = this->DEFAULT_DX;
    this->dy = this->DEFAULT_DY;
}

int Player::getLeft() {
    return this->left;
}

int Player::getTop() {
    return this->top;
}

int Player::getRight() {
    return this->left + this->SIZE;
}

int Player::getBottom() {
    return this->top + this->SIZE;
}

int Player::getScore() {
    return this->score;
}

double Player::getDx() {
    return this->dx;
}

double Player::getDy() {
    return this->dx;
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

void Player::resetSpeed() {
    this->dx = this->DEFAULT_DX;
    this->dy = this->DEFAULT_DY;
}

void Player::jump() {
    this->dy = -4 * this->DEFAULT_DY;
}

void Player::timer() {
    this->left += this->dx;
    this->top += this->dy;
    
    if (this->dy < this->DEFAULT_DY)
        this->dy += 0.5 * this->DEFAULT_DY;
}

void Player::draw(int offset) {
    al_draw_bitmap(this->bitmap, this->left + offset, this->top, 0);
}
