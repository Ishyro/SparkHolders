#include "src/data/Projectile.h"

int Projectile::getX() {return x;}
int Projectile::getY() {return y;}
int Projectile::getOrientation() {return orientation;}

int Projectile::getRawDamage() {
  int power = 0;
  for (int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
  }
  return power;
}

int Projectile::getDamageType(int damage_type) {return damages[damage_type];}

int Projectile::getSpeed() {return speed;}

// call only when no collision
void Projectile::move() {
  switch(orientation) {
    case NORTH:
      y += speed;
      break;
    case NORTH_EAST:
      y += speed;
      x += speed;
      break;
    case EAST:
      x += speed;
      break;
    case SOUTH_EAST:
      y -= speed;
      x += speed;
      break;
    case SOUTH:
      y -= speed;
      break;
    case SOUTH_WEST:
      y -= speed;
      x -= speed;
      break;
    case WEST:
      x -= speed;
      break;
    case NORTH_WEST:
      y += speed;
      x -= speed;
      break;
  }
}

void Projectile::move(int orientation) {this->orientation = orientation;}
void Projectile::move(int x, int y, int orientation) {this->x = x; this->y = y; this->orientation = orientation;}
