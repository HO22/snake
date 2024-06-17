#ifndef POSITION_H
#define POSITION_H
#include <chrono>
#include <random>
#include <vector>
using namespace std;

enum Direction { Up, Right, Down, Left };

class Position {
 public:
  Position(int _x = 0, int _y = 0) : x(_x), y(_y){};
  Position(const Position& p) : x(p.x), y(p.y){};

  void setRandomPosition(const int max_x, const int max_y) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_x(0, max_x - 1);
    uniform_int_distribution<> dis_y(0, max_y - 1);

    int _x = dis_x(gen);
    int _y = dis_y(gen);

    x = _x;
    y = _y;
  };

  vector<Position> nextPosition() {
    vector<Position> v_p;
    v_p.push_back(Position(x - 1, y + 0));
    v_p.push_back(Position(x + 1, y + 0));
    v_p.push_back(Position(x + 0, y - 1));
    v_p.push_back(Position(x + 0, y + 1));
    return v_p;
  };

  Position nextPosition(Direction& d) {
    switch (d) {
      case Up:
        // cout << "U" << endl;
        // cout << "<" << x << " " << y - 1 << ">" << endl;
        return Position(x, y - 1);
        break;
      case Down:
        // cout << "D" << endl;
        // cout << "<" << x << " " << y + 1 << ">" << endl;
        return Position(x, y + 1);
        break;
      case Left:
        // cout << "L" << endl;
        // cout << "<" << x - 1 << " " << y << ">" << endl;
        return Position(x - 1, y);
        break;
      case Right:
        // cout << "R" << endl;
        // cout << "<" << x + 1 << " " << y << ">" << endl;
        return Position(x + 1, y);
    };
  };

  bool operator==(const Position& p) const { return (x == p.x) && (y == p.y); };
  bool operator!=(const Position& p) const { return !(*this == p); };
  int x;
  int y;
};

// int main() {
//   Position p(0, 0);
//   vector<Position> n_p;
//   n_p = p.nextPosition();
// }

#endif