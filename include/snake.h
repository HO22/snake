#ifndef SNAKE_H
#define SNAKE_H

#define INIT_LENGTH 3
#define MIN_LENGTH 2
#include <deque>
#include <iostream>
using namespace std;

#include "position.h"

class Snake {
 public:
  Snake(int m_l = 0) : body(), len(0), direction(Left), maxLen(m_l){};
  Snake(int m_l, const deque<Position>& _body);
  Snake(const Snake& s)
      : body(s.body), len(s.len), direction(s.direction), maxLen(s.maxLen){};

  /* GETTER */
  const int getLength() { return body.size(); };
  const int getMaxLength() { return maxLen; };
  const deque<Position>& getBody() const { return body; };
  const Position getHead() const { return body.back(); };
  const Position getTail() const { return body.front(); };
  const Direction getDirection() { return direction; };

  /* SETTER */
  void setDirection(const Direction d) {
    direction = d;
    switch (direction) {
      case Down:
        // cout << "DOWN" << endl;
        break;
      case Up:
        // cout << "Up" << endl;
        break;
      case Left:
        // cout << "Left" << endl;
        break;
      case Right:
        // cout << "Right" << endl;
        break;
    }
  };

  void move(const Position& p);
  void incBody(const Position& p);
  void decBody(const Position& p);

 private:
  int len;
  int maxLen;
  int minLen;
  deque<Position> body;
  Direction direction;
};

#endif