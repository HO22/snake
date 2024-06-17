#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <map>
#include <string>

#include "position.h"

enum ItemKind {
  GROWTH,
  POISON,
};

class Item {
 public:
  Item(){};
  Item(const Position& p, const ItemKind k) : position(p), kind(k){};

  const Position getPosition() const { return position; };
  const ItemKind getKind() const { return kind; };

 private:
  Position position;
  ItemKind kind;
};

#endif