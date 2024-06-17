
#ifndef GATE_H
#define GATE_H

#include "position.h"

class Gate {
 public:
  Gate();
  Gate(const Position& pos);

  Position getPosition() const { return position; }

 private:
  Position position;
  // Direction exitDirection;

  // void determineExitDirection();
  // Position determineExitPosition(const Position& gate1_pos,
  //                                Direction gate1_exit_dir);
  // Direction getNextDirection(Direction current_dir, bool clockwise);
};

#endif