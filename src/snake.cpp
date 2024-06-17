#include "../include/snake.h"

#include "../include/position.h"
using namespace std;

Snake::Snake(int m_l, const deque<Position>& _body)
    : body(_body), len(_body.size()), direction(Right), maxLen(m_l) {}

void Snake::move(const Position& p) {
  body.push_back(p);
  body.pop_front();
  // deque<Position>::iterator it = body.begin();
  // for (; it != body.end(); it++) {
  //   cout << it->x << " " << it->y << endl;
  // }
};

void Snake::incBody(const Position& p) { body.push_back(p); };

void Snake::decBody(const Position& p) {
  body.push_back(p);
  body.pop_front();
  body.pop_front();
}