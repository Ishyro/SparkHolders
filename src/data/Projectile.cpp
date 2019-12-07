#include "src/data/Projectile.h"

int Projectile::getX() {return x;}
int Projectile::getY() {return y;}
int Projectile::getOrientation() {return orientation;}
int Projectile::getPower() {return power;}
int Projectile::getSpeed() {return speed;}

void Projectile::move(int orientation) {this->orientation = orientation;}
void Projectile::move(int x, int y, int orientation) {this->x = x; this->y = y; this->orientation = orientation;}
